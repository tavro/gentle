#ifndef IMAGE_H
#define IMAGE_H

#include "./ui_object.h"

class Image : public UIObject
{
	public:
		Image( int x, int y, int w, int h );
};

#endif // IMAGE_H