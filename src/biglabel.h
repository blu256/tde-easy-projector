#ifndef __BIGLABEL_H__
#define __BIGLABEL_H__

#include <tqlabel.h>

class BigLabel : public TQLabel
{
	TQ_OBJECT

	public:
		BigLabel(const TQString& text, TQWidget *parent);
		~BigLabel();

		void setTip(const TQString& text);
};

#endif // __BIGLABEL_H__
