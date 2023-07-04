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

templates_path = [ "_templates" ]
exclude_patterns = [ "_build", "Thumbs.db", ".DS_Store" ]


# -- Options for HTML output -------------------------------------------------

html_static_path = [ "_static" ]

html_theme = "sphinx_rtd_theme"
html_theme_options = {
    "collapse_navigation": True,
    "sticky_navigation": True,
    "navigation_depth": 4,
    "includehidden": False,
    "titles_only": False,
    "display_version": True,
    "logo_only": False,
    "prev_next_buttons_location": None,
    "style_external_links": True
}

html_title = "Thallium Docs"


# -- Breathe configuration ---------------------------------------------------

breathe_default_project = "Thallium"
