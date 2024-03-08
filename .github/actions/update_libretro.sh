set -ex

git fetch --tags
LATEST=$(git tag --sort=-creatordate | grep "^v" | grep -v libretro | head -n 1)

if [ $(git tag -l "$LATEST"-libretro) ]; then
    echo "The libretro branch is already up-to-date"
    exit 0
fi

echo "Building boot ROMs..."
make -j bootroms
mv build/bin/BootROMs BootROMs/prebuilt

echo "Updating branch"
git config --global --add --bool push.autoSetupRemote true
git config --global user.name 'Libretro Updater'
git config --global user.email '<>'

git branch --delete libretro || true
git checkout tags/$LATEST -b libretro

git add BootROMs/prebuilt/*
git commit -m "Update libretro branch to $LATEST"
git tag "$LATEST"-libretro
git push --force
git push --tags
