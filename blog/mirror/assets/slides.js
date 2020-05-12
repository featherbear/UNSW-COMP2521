/**
 * Slide Engine
 * derived from jas' slide code
 *
 * @author Jashank Jeremy <jashankj@cse.unsw.edu.au>
 * @date 2017-09-07
 */

'use strict';

var $presenter = null;

// a presentation tool
function Presenter() /*: void */ {
	// Presentation variables.
	this.curr_slide = 0;
	this.max_slides = -1;
	this.slides = null;
	this.toc_element = null;
}

// present a slide. naive deselection, so this is probably extremely
// slow if there are _lots_ (~thousands) of slides; has to traverse
// them all, but eh.  if that happens, I'll fix it.
//
// the slide counter is zero indexed; this is not a bug.
Presenter.prototype._slide = function() /*: void */ {
	if (this.curr_slide == null)
		return;

	for (var i = 0; i < this.slides.length; i++)
		this.slides.item(i).classList.remove('active');

	var slide /*: HTMLElement */ =
		this.slides.item(this.curr_slide);
	slide.classList.add('active');

	var count /*: HTMLElement */ =
		document.getElementById('slide-counter');
	count.innerText = this.curr_slide.toString();

	var navs /*: HTMLCollection<HTMLElement>*/ =
		document.querySelectorAll('.presentation-sections li a');
	for (var i = 0; i < navs.length; i++) {
		var sl = parseInt(navs.item(i).getAttribute('data-slide'));
		var sl_ = (i < (navs.length - 1)) ?
			parseInt(navs.item(i+1).getAttribute('data-slide')) :
			this.max_slides;

		if (sl <= this.curr_slide && this.curr_slide < sl_)
			navs.item(i).classList.add('active');
		else
			navs.item(i).classList.remove('active');
	}

	window.location.hash = '#' + this.curr_slide.toString();
};

// show the next slide.
Presenter.prototype.next = function() /*: void */ {
	if (this.curr_slide == null)
		return;

	if (this.curr_slide < (this.max_slides - 1))
		this.curr_slide += 1;

	this._slide();
};

// show the previous slide.
Presenter.prototype.prev = function() /*: void */ {
	if (this.curr_slide == null)
		return;

	if (this.curr_slide > 0)
		this.curr_slide -= 1;

	this._slide();
};

// start a presentation (inits stuff).
Presenter.prototype.start = function(cs_hint /*: number*/) /*: void */ {
	this.curr_slide = (cs_hint != undefined) ? cs_hint : 0;
	this.max_slides = this.slides.length;

	document.querySelector('body').classList.remove('previewing');
	document.querySelector('body').classList.add('presenting');

	// Set up the key press listener.
	// Cribbed from jas' slide show code, and modernised a bit.
	document.addEventListener('keypress', function(ev) /*: void */ {
		// something's gone horribly wrong
		if ($presenter == null)
			return;

		switch (ev.key) {
		case ' ':
		case 'PageDown':
		case 'n':
			$presenter.next();
			break;

		//case '': // The literal ^H (0x08)
		case 'PageUp':
		case 'b':
			$presenter.prev();
			break;
		}
	});

	this._slide();
};

Presenter.prototype.go_to = function(idx /*: number*/) /*: void */ {
	this.curr_slide = (idx != undefined) ? idx : 0;
	this._slide();
}

Presenter.prototype._setup_section_nav = function() /*: void */ {
	this.section_navs = document.createElement('ul');
	this.section_navs.classList.add('presentation-sections');

	var sects = document.querySelectorAll('section[name]');
	for (var i = 0; i < sects.length; i++) {
		var sect_a = document.createElement('a');
		var target = 
			(sects[i].childElementCount > 0) ?
			sects[i].children[0].getAttribute('data-slide-index') :
			0;
		sect_a.href = '#' + target;
		sect_a.textContent = sects[i].getAttribute('name');
		sect_a.setAttribute('data-slide', target);

		var sect_li = document.createElement('li');
		sect_li.appendChild(sect_a);

		this.section_navs.appendChild(sect_li);
	}

	var slide_deck = document.querySelector('.slide-row');
	slide_deck.insertBefore(this.section_navs, this.toc_element);

	var headers =
		document.querySelectorAll('.presentation-sections li a');
	for (var i = 0; i < headers.length; i++)
		headers[i].addEventListener(
			'click', function(ev /* EventTarget */) /*: bool */ {
				$presenter.go_to(
					parseInt(ev.target.getAttribute('data-slide')));
				return false;
			});
}

Presenter.prototype._setup_toc = function() /*: void */ {
	this.toc_element = document.createElement('ol');
	this.toc_element.classList.add('print-toc');
	this.toc_element.setAttribute('start', 0);

	var slide_deck = document.querySelector('.slide-row');
	for (var i = 0; i < this.slides.length; i++) {
		var slide_title = this.slides[i].querySelector('h1, h2, h3');
		var slide_title_text =
			(slide_title != null) ?
			slide_title.textContent :
			'[...]';
		var slide_title_sect =
			(this.slides[i].parentElement.nodeName === "SECTION" &&
			 this.slides[i].parentElement.hasAttribute('name')) ?
			(this.slides[i].parentElement.getAttribute('name') + ': ') :
			'';

		var slide_li = document.createElement('li');
		slide_li.textContent = slide_title_sect + slide_title_text;
		this.toc_element.appendChild(slide_li);
	}
	slide_deck.insertBefore(this.toc_element, this.slides[0]);
}

// on-load hook.
Presenter.prototype.on_load = function() /*: void */ {
	this.slides = document.querySelectorAll('main[is="slide"]');

	for (var i = 0; i < this.slides.length; i++)
		this.slides[i].setAttribute('data-slide-index', i.toString());

	this._setup_toc();
	this._setup_section_nav();

	// if the url's of the form #14, go to slide index 14.
	if (window.location.hash) {
		var hash =
			window.location.hash
			.toString()
			.match(/^#(\d+)$/);
		if (! (hash === null && hash[1] === undefined))
			this.start(parseInt(hash[1]))
	}
};

////////////////////////////////////////////////////////////////////////
// When we load:
window.onload = function() /*: null */ {
	$presenter = new Presenter();
	$presenter.on_load();
};


/* Local variables: */
/* tab-width: 4 */
/* indent-tabs-mode: t */
/* End: */
