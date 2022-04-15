#include <tqtooltip.h>

#include <tdeapplication.h>
#include <kiconloader.h>

#include "bigbutton.h"


BigButton::BigButton(TQWidget *parent, const TQString& icon, const TQString& label, TQObject *receiver, const char *slot)
	: TQToolButton(parent)
{
	setUsesBigPixmap(true);
	setUsesTextLabel(true);
	setToggleButton(true);
	setTextPosition(TQToolButton::BelowIcon);

	setTextLabel(label, false);
	setIconSet(kapp->iconLoader()->loadIconSet(icon, TDEIcon::User, TDEIcon::SizeEnormous));

	connect(this, SIGNAL(toggled(bool)), receiver, slot);
}

BigButton::~BigButton()
{
}

void BigButton::setTip(const TQString& text)
{
	TQToolTip::remove(this);
	TQToolTip::add(this, text);
}

#include "bigbutton.moc"
