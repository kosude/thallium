/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#import "cocoa_mtbridge.h"

#import <QuartzCore/QuartzCore.h>

id TL_NSWindowGetView(id window) {
    return [window contentView];
}

id TL_NSViewGetMetalLayer(id view) {
    id layer = [CAMetalLayer layer];
    if (!layer) {
        return NULL;
    }

    [view setLayer:layer];
    [view setWantsLayer:YES];

    return layer;
}
