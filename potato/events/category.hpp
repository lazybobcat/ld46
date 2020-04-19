#ifndef CATEGORY_H
#define CATEGORY_H

namespace Category
{
    enum Type
    {
        None            = 0,
        SoundEffect     = 1 << 0,
        ParticleSystem  = 1 << 1,
        Player          = 1 << 2,
        Field           = 1 << 3,
        Well            = 1 << 4,
        Points          = 1 << 5,
        // ...
        //MixedCategory        = Category1 | Category2,

    };
}

#endif // CATEGORY_H
