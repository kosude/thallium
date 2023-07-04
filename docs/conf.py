# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html


# -- Project information -----------------------------------------------------

project = "Thallium"
copyright = "2023 Jack Bennett"
author = "Jack Bennett"


# -- General configuration ---------------------------------------------------

extensions = [ "breathe" ]

templates_path = [ "res/templates" ]
exclude_patterns = [ "_build", "Thumbs.db", ".DS_Store" ]

master_doc = "pages/index"


# -- Options for HTML output -------------------------------------------------

html_static_path = [ "res/static" ]

html_theme = "sphinx_rtd_theme"
html_theme_options = {
    "collapse_navigation": True,
    "sticky_navigation": True,
    "navigation_depth": 4,
    "includehidden": False,
    "titles_only": False,
    "display_version": True,
    "logo_only": True,
    "prev_next_buttons_location": None,
    "style_external_links": True
}

html_title = "Thallium"
html_favicon = "res/static/img/favicon.png"
html_logo = "res/static/img/logo_wtext.svg"


# -- Breathe configuration ---------------------------------------------------

breathe_default_project = "Thallium"
