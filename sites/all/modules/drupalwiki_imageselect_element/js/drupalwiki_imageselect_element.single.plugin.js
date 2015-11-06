// $Id$
/**
 * @author Eugen Mayer (http://kontextwork.de)
 * @Copyright 2010 KontextWork
 */
(function ($) {
  $.fn.drupalwiki_imageselect_element_single = function () {
    return this.each(function () {
        // **** our hepler functios *********
        function insert_container() {
          $parent.after('<div id="' + container + '" class="drupalwiki_imageselect_element_container clearfix"></div>');
        }

        this.show_selected = function() {
          var $option = $($parent.find('option:selected').get(0));
          // we use text so we actually decode what there, so <img> shows up as an image
          var $el = $('<span class="select_option" key="' + $option.val() + '">' + $option.text() + '</span>');
                /*.bind('click',
                function () {
                  // set our parents value, so on form submit
                  // the right selection is submitted
                  select_parent_option(key);
                  if (show_labels) {
                    show_parent_label(key);
                  }
                })*/

           $container.html($el);
        }

        function select_parent_option(key) {
          $parent.val(key);
          // update the selection in our special UI
          show_selected();
        }

        function show_parent_label(key) {
          var title = $container.find('span.select_option[key="' + key + '"] img').attr('title');
          $container.find('span.image_title').remove();
          $container.append('<span class="image_title">' + title + '</span>');
        }

        // **************** Init *****************/
        var $parent = $(this);
        var plugin = this;
        // we are done, so set us to processed
        $parent.addClass('drupalwiki-imageselect-element-single-processed');
        var context = $parent.attr('id');
        var container = context + '-container';

        var show_labels = false;

        if (context === null) {
          alert('cant initialize drupalwiki-imageselect-element-single: ' + context);
          return;
        }

        $parent.hide();
        // Lets set all things up.
        insert_container();
        var $container = $('#' + container);
        this.show_selected();

        if (show_labels) {
          var firstkey = $('#' + context + ' :selected').attr('value');
          show_parent_label(firstkey);
        }

        $parent.bind('change',function() {
          plugin.show_selected();
        });
      }
    );
  }
})(jQuery);