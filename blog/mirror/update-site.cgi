#!/web/cs1511/.ruby/bin/ruby

require 'openssl'

SITE_ROOT = '/web/cs2521/19x1/source'
ACTIVITY_DIR = File.join SITE_ROOT, '_activities'
TOKEN_FILE = File.join SITE_ROOT, '.webhook_key'
TOKEN = File.open(TOKEN_FILE) { |f| f.read.strip }

CONTENT_TYPE = 'text/plain; charset=utf-8'

# Are we testing?
LOCAL = false

def verify_signature(body, token)
  return true if LOCAL

  sig = ENV['HTTP_X_HUB_SIGNATURE'].split('=', 2).last
  check = OpenSSL::HMAC.hexdigest OpenSSL::Digest::SHA1.new, token, body
  sig == check
end

body = stdin.read()

unless verify_signature body, TOKEN
  print <<__EOF__
Status: 400
Content-Type: #{CONTENT_TYPE}

Error verifying token
__EOF__
  exit
end

# Write a string with a CRLF line terminator.
def puts_crlf(msg)
  print msg + "\r\n"
end

puts_crlf "Content-Type: #{CONTENT_TYPE}"
puts_crlf ""

body_json = JSON.load body

unless body_json['ref'] == 'refs/heads/master'
  # Not a push to master
  puts_crlf "Ignoring push to #{body_json['ref']}"
  exit(0)
end

def unwords(xs)
  xs.join(' ')
end

def run(xs)
  system unwords xs
end

puts_crlf "=== updating repository"
run %w{git pull -r} unless LOCAL

puts_crlf "=== rebuilding site..."
run %w{rake tools:activity2web site:build site:sync}

puts_crlf "=== site built!"
