# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import sys
import os
sys.path.append(os.path.dirname(__file__))

import conf_utils


# -- Project information -----------------------------------------------------

project = "Thallium Documentation"
copyright = "2023 Jack Bennett"
author = "Jack Bennett"

# hardcoded prerelease for now as currently all versions are prereleases
release = conf_utils.get_version(True, True)
version = release


# -- General configuration ---------------------------------------------------

extensions = [ "breathe" ]

templates_path = [ "res/templates" ]
exclude_patterns = [ "_build", "Thumbs.db", ".DS_Store" ]

master_doc = "index"


# -- Options for HTML output -------------------------------------------------

html_static_path = [ "res/static" ]

html_theme = "sphinx_rtd_theme"
html_theme_options = {
    "collapse_navigation": True,
    "sticky_navigation": True,
    "navigation_depth": 4,
    "titles_only": False,
    "display_version": True,
    "logo_only": False,
    "prev_next_buttons_location": None,
    "style_external_links": True
}

html_title = "Thallium"
html_favicon = "res/static/img/favicon.png"
html_logo = "res/static/img/logo_wtext.svg"

html_css_files = [
    "css/dark.css",
    "css/see_also_hotfix.css",
    "css/sidebar.css",
]

html_show_sphinx = False

html_sidebars = { "**": [ "globaltoc.html", "relations.html", "sourcelink.html", "searchbox.html" ] }


# -- Breathe configuration ---------------------------------------------------

breathe_projects = { "Thallium": "../build/docs/xml" }
breathe_default_project = "Thallium"

breathe_show_define_initializer = True
breathe_show_enumvalue_initializer = True

breathe_domain_by_extension = {
    "h": "c"
}
