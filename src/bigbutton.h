#ifndef __BIGBUTTON_H__
#define __BIGBUTTON_H__

#include <tqtoolbutton.h>

class BigButton : public TQToolButton
{
	TQ_OBJECT
	
	public:
		BigButton(TQWidget *parent, const TQString& icon, const TQString& label, TQObject *receiver, const char *slot);
		~BigButton();

		void setTip(const TQString& text);
};

#endif // __BIGBUTTON_H__
