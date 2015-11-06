// $Id

= OG username helper =

== Overview ==

This module try to help OG administrators to add site users to their OG
by providing an autocomplete input(like the one you can see editing core
node user author).

This module start as a patch for OG: http://drupal.org/node/225308[More
convenient way to add users]. And almost 2 years after, it seems to be a
good idea to maintain it as a separate module.

You can see it in action: http://blip.tv/file/2921743

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/og_username_helper


== Requirements ==

* Drupal modules:
** Organic groups: http://drupal.org/project/og

== Installation ==

* Install as usual, see http://drupal.org/node/70151[Installing
  contributed modules] for further information.


== Configuration ==

* Configure user permissions in Administer >> User management >>
  Permissions >> og_username_helper module:
** access og username helper
+
Users in roles with the 'access og username helper' permission will see
the autocomplete input to insert usernames.


== Contact ==

Current maintainers:

* Marco Villegas - http://drupal.org/user/132175[marvil07]

