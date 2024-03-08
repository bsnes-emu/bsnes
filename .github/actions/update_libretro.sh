set -ex

git fetch --tags
LATEST=$(git tag --sort=-creatordate | grep "^v" | grep -v libretro | head -n 1)

if [ $(git tag -l "$LATEST"-libretro) ]; then
    echo "The libretro branch is already up-to-date"
    exit 0
fi

git config --global --add --bool push.autoSetupRemote true
git config --global user.name 'Libretro Updater'
git config --global user.email '<>'

cp libretro/gitlab-ci.yml .gitlab-ci.yml

echo "Switching to tag $LATEST"
git branch --delete libretro || true
git checkout tags/$LATEST -b libretro

echo "Building boot ROMs..."
make -j bootroms

echo "Updating branch"
mv build/bin/BootROMs BootROMs/prebuilt
git add BootROMs/prebuilt/* .gitlab-ci.yml
git commit -m "Update libretro branch to $LATEST"
git tag "$LATEST"-libretro
git push --force
git push --tags
