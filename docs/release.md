Release checklist
=================

 1. Commit the new release
 2. Tag the commit
 3. `git push --tags origin master` to push the commit and tag at the
    same time.
 5. Go to [the docs admin][rtd] and verify that it's building the new
    version as 'stable' and under its tag name.
 
 [rtd]: https://readthedocs.org/projects/higan/builds/
