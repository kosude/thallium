/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__wsi__cocoa_mtbridge_h__
#define __TL__internal__wsi__cocoa_mtbridge_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#if defined(__OBJC__)
#   import <Cocoa/Cocoa.h>
#else
    typedef void *id;
#endif

/**
 * @brief Get the NSView object for the given Cocoa NSWindow.
 *
 * This function returns the NSView object reference for the given NSWindow reference.
 *
 * @param window The NSWindow from which to retrieve the view.
 * @return NSView object reference, or NULL if there was an error.
 */
id TL_NSWindowGetView(
    id window
);

/**
 * @brief Get a CAMetalLayer reference from the given Cocoa NSView.
 *
 * This function returns a CAMetalLayer reference from the given Cocoa NSView.
 *
 * @param view The NSView from which to infer the Metal layer.
 * @return CAMetalLayer object reference, or NULL if there was an error!
 */
id TL_NSViewGetMetalLayer(
    id view
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
