(function($) {

  Drupal.behaviors.morecomments = {
    attach: function() {
      var pager = Drupal.settings.morecomments_pager;
      // Replace the default pager with the more comments pager
      $("#comments .pager").replaceWith(pager);

      $(".morecomments-button").on('click', morecomments_load);

      function morecomments_load(event) {
        var classes = $(this).attr("class");
        var info = classes.match("node-([0-9]+) page-([0-9]+)");
        $(".morecomments-button").append("<div class = 'wait'>&nbsp;</div>");
        $.get(Drupal.settings.basePath + "morecomments/" + info[1] + "/" + info[2], function(data) {
          var parent = $(".morecomments-button").parent();
          $(".morecomments-button").replaceWith(data);
          Drupal.attachBehaviors(parent);
        });
      }
    }
  };

})(jQuery);
