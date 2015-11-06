/**
 * $Id: README.txt,v 1.3.2.1 2010/09/15 02:04:31 rconstantine Exp $
 * @package CCK_Fullname
 */
This module adds a full name CCK field to your installation. There is a set of five subfields,
prefix, first, middle, last, and suffix. There are several configuration options allowing you
to use present the names in different formats.

--CONTENTS--
  REQUIREMENTS
  INSTALL/SETUP
  FEATURES
  USAGE (Example)
  FUTURE
  UNINSTALL
  CREDITS
  MIGRATION
  HELP
  
--REQUIREMENTS--
  This module requires the CCK content module.
    
--INSTALL/SETUP--
  Download and ungzip/untar the package and place it wherever you have you other modules installed.
      
--FEATURES--
  A set of five subfield names: prefix, first, middle, last and suffix.
  
  You may turn any of these subfields on or off. You may REQUIRE individual subfields without REQUIRING
  the entire field.

  --If you have REQUIRED the field without REQUIRING subfields, then the user must fill out at least
  one subfield with no restrictions on which field.

  --If you have REQUIRED subfields but not the field itself then those restrictions will only be applied
  when the user enters in at least a first or last name (both of these being empty is the definition of
  an empty set of subfields and will not be saved).

  --If you have REQUIRED subfields and the field itself, then the user must enter at least one name AND
  it must conform to your subfield settings.
  
  You may specify the maximum length of every field.
  
  You may limit middle names to an initial.
  
  Multiple values works in the standard Drupal/CCK way.

  IN CASE YOU MISSED IT: the subfields FIRST and LAST define whether the whole field is empty since it
  doesn't make much sense to store just a prefix, just a middle name or initial, or just a suffix. So
  all standard Drupal/CCK behaviors associated with empty fields should work when both of those subfields
  are empty. If either is full, the field is not empty.
  
--USAGE--
  ADMINS-------
    Create a content type as you would normally. See the CCK docs for more info.
    
    Add a content type. Name it as you wish and select the 'fullname' widget. Save your choice. You
    are then presented with the settings page.
    (ex. admin/content/node-type/[your content type]/fields/[your field name])
    
      Fill the upper section as you see fit, then turn your attention to the bottom section labeled
      'Global settings'. Here you will find all of the admin settings for this cck field set.
    
      At the top is a the standard CCK 'Required' checkbox and this governs the field as a whole per
      the discussion above under FEATURES.

      Next is the standard CCK 'Number of Values' dropdown. Any value is valid for this module.
    
      Next is a check box is for restricting the middle name to 1 character, for storage as an initial.
      This is probably redundant since you could manually specify a field size of 1 below.
    
      The next section is 'Available Parts' and has several checkboxes in it. The check boxes allow
      you to show/hide fields for input. If you don't care about prefixes and suffixes, for example,
      just uncheck them. Default is unchecked so be sure to visit this section for all new fields.
    
      The next section is 'Required Parts' and lists all of the subfields. Checking these boxes
      makes them individually REQUIRED. See the FEATURES section above for a description of how they
      work.
    
      In the next section, you may specify the maximum lengths of all subfields.

    Next, you should visit the Display Fields page of your content type.
    (ex. admin/content/node-type/[your content type]/display)

      Here you can choose between 5 display options.
      --The 'default' option gives you: Prefix First Middle Last Suffix
      --The 'Last name only' option gives you: Last
      --The 'First name only' option gives you: First
      --The 'Last, first short' option gives you: Last, First Middle
      --The 'Last, first long' option gives you: Last Suffix, Prefix First Middle

  USERS-------
    From a user's perspective, there really isn't much to see. At node creation/editing, the field
    is presented to the user in standard CCK format. By default, the subfields are stacked and each
    new field addss new stacked subfields below existing ones. This can make for a long page, so
    you may want to use this CSS in your style sheet:

    .cck_fullname_prefix_wrapper {
      float: left;
      margin-left: 5px;
    }

    .cck_fullname_first_wrapper {
      float: left;
      margin-left: 5px;
    }

    .cck_fullname_middle_wrapper {
      float: left;
      margin-left: 5px;
    }

    .cck_fullname_last_wrapper {
      float: left;
      margin-left: 5px;
    }

    .cck_fullname_suffix_wrapper {
      float: left;
      margin-left: 5px;
    }
    
  THEMES-------
    I have made an effort to provide DIVs and classes where appropriate. Unfortunately, to get
    the validation to work, I had circumvent the FAPI #required element and use my own. FAPI
    would not allow you to leave fields empty and have them drop off according to CCK's empty
    rules. Consequently, I have copied two themes from the Drupal forms.inc file and changed
    only two lines, one in each. The change in the first theme function is a call to the other.
    The second one places the red REQUIRED asterisk in the right place. Without doing this,
    options were limited and looked terrible.
    
--FUTURE--
  I would like to get this VIEWS compatible, TOKEN compatible, and DIFF compatible.
  
--UNINSTALL--
  Just disable the module, then uninstall. CCK should do its thing to remove any remnants
  except for the standard CCK exceptions.
  
--CREDITS--
  This module developed by Ryan Constantine (rconstantine). Thanks to linuxbox for ideas and 
  some code in the old D5 version.

  Big thanks to Fertility Authority out of New York for sponsoring the D6 version of this!!!
  
--MIGRATION--
  If you are upgrading from D5, I'm not sure what will and won't work. It may be that CCK will
  handle some stuff for us. You'll notice that there is now just one set of subfields whereas
  before there was a set for a LEGAL NAME and a set for PREFERRED NAME. Now we don't have that
  in favor of CCK's current way of handling multiple fields.

  If you only used LEGAL NAMES, after you upgrade to D6, you should just delete the columns in
  your database related to the PREFERRED NAMES since these are no longer used. Then empty your
  cache_content table.

  If you only used PREFERRED NAMES, after you upgrade to D6, you should delete the LEGAL NAMES
  columns in your database, then rename the left over (PREFERRED) ones to match what the LEGAL
  ones were since the new module uses those. Empty your cache_content table.

  If you used both LEGAL and PREFERRED NAMES, you should create a new fullname field on D5
  before you upgrade to D6, set it up to use LEGAL NAMES, and migrate your PREFERRED NAMES data
  into its table and columns. This way, you will have two fullname fields using only LEGAL NAMES
  and you can follow the process above for that.

  It may be that you will need to change/resave the settings for each CCK field, so before you
  try to manipulate and old data that you migrated, visit the Configure page for each fullname
  field and save it. If you have the luxury of a test system, setup a fresh D6 install and
  mimick your setup from D5. Then after your D6 upgrade on your D5 system, you can compare the
  database entries for each and make sure they match. In particular, look at the
  content_node_field and content_node_field_instance tables.

  Hopefully these tips will work for you.
  
--HELP--
  As usual, go to http://drupal.org/project/issues/cck_fullname to post issues of support, bug
  reports, and feature requests. Contribute to the code if you can.