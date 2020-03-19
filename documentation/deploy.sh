#!/bin/sh

set -e

#mkdocs gh-deploy --clean --message "new documentation version"

mkdocs build
doxygen Doxyfile

git stash
git checkout gh-pages
git stash pop

cp -r site/* ../

git commit -a -m "new documentation version"

git stash
git checkout master
git stash pop
