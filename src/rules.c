/*
DECISION:PERM:USER1,…,USERN:GROUP1,…,GROUPN:FILEPATH1,…,FILEPATHN

DECISION is 'allow', 'deny' or 'deny_audit'.
PERM is 'open', 'execute' or 'any'
USER is a username (e.g. smithm), or * to match all users
GROUP is a groupname (e.g. wheel), or * to match all groups
FILEPATH could be:
  1. The full path to an individual file (e.g. /etc/passwd)
  2. The full path to an individual directory (e.g. /var/log/ <— trailing slash indicates it’s a directory, not a file)
  3. The full path with wildcards (e.g. /var/www/html/*.html)
FILEPATH matches will leverage POSIX fnmatch() with flags set as FNM_PATHNAME|FNM_PERIOD (The shell uses the FNM_PERIOD and FNM_FILE_NAME flags together for matching file names so it seems logical that our matches behave the same way as the shell).
Like fapolicyd, rules will be evaluated by the rules engine in-order until the first match (sieve style evaluation).

Use-case example:

Suppose we have a web server running and we want the web development team to have access to all files in /var/www/ except for an account database that resides at /var/www/accounts/account.db, sample rules will look like this:

deny:open::webdev:/var/www/accounts/account.db
allow:open:root,apache:root,apache,webdev:/var/www/*
deny:any:*:*:*

*/
