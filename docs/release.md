Release checklist
=================

 1. Commit the new release
 2. Tag the commit
 3. `git push --tags origin master` to push the commit and tag at the
    same time.
 4. Go to [the docs admin][rtd] and verify that it's building the new
    version as 'stable' and under its tag name.
 5. Check out the `libretro` branch.
 6. Merge changes from master.
 7. Copy `target-bsnes/resource/resource.?pp` to the `target-libretro` folder.
 7. `git push` to make the new changes available.
 
 [rtd]: https://readthedocs.org/projects/higan/builds/
