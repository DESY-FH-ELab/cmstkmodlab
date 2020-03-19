### Doxygen documentation

#### Manually generate Doxygen pages in HTML format

Execute the following command from the base directory of the project:
```
doxygen Doxyfile
```
This will create a directory called `site/doxygen`. You can open the HTML documentation with a browser, doing for example
```
firefox site/doxygen/index.html &
```

#### Upload Doxygen documentation to [GitHub Pages](https://help.github.com/en/github/working-with-github-pages) (branch: `gh-pages`)

The Doxygen documentation can be updated and uploaded to GitHub Pages by executing
(requires `mkdocs`, `doxygen`, and a branch called `gh-pages`)
```
./deploy.sh
```
