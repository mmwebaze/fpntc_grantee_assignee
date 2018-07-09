# fpntc_grantee_assignee
Assigns grantee to a user.

## Installation

<ol>
<li>lando drush en fpntc_grantee_assignee -y</li>
<li>Make sure the field <b>zip codes (machine name: field_zip_codes)</b> has been added to the <b>grantees</b> vocabulary</li>
</ol>

##What the module does and how to use it

<ul>
<li>Migrates taxonomies from csv file located in the <b>public://</b> folder. Run: lando drush gac</li>
<li>Updates users with the appropriate grantee based on their zip code. Run: lando drush gau</li>
<li>Assigns grantee to new registered user based on their zip code</li>
</ul>