
Overview
--------
This module allows administrators an avenue to manage free tagging terms while
still encouraging users to suggest new terms.

Examples of potential usage:

  * If a tag added by a user is a synonym of an existing term, (optionally and)
    automagically replace it with the existing term. This can also be used to
    handle alternate spellings of a term, typographical errors and so on.
  * If a tag added by a user uses the related terms feature of the taxonomy
    module, optionally tag the node with those terms as well.
  * Optionally also insert the ancestors (parents, grandparents etc.) of the
    provided tags.
  * Make a free-tagging vocabulary read-only. Non-existent terms are not
    included and are instead sent to a management interface where they can be
    dealt with appropriately.

The management interface mentioned above includes the following features:

  * Save as new terms.
  * Save as synonyms of provided root terms.
  * Save as child terms of provided parent terms.
  * Delete

Any new terms which are approved are also automatically associated with the
nodes that suggested them in the first place.

Links
-----
  * Administration page: admin/content/taxonomy/relatedlinks
  * Project page: http://drupal.org/project/unitag

Author(s)
---------
Karthik Kumar / Zen / |gatsby| : http://drupal.org/user/21209
