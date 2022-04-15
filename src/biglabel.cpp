#include <tqtooltip.h>

#include <tdeglobalsettings.h>

#include "biglabel.h"

BigLabel::BigLabel(const TQString& label, TQWidget *parent)
	: TQLabel(label, parent)
{
	setAlignment(TQt::AlignCenter);

	//setFont(TDEGlobalSettings::largeFont());
	TQFont font = TDEGlobalSettings::generalFont();
	font.setPointSize(font.pointSize() * 2);
	setFont(font);

	setPaletteBackgroundColor(TDEGlobalSettings::highlightColor());
	setPaletteForegroundColor(TDEGlobalSettings::highlightedTextColor());

	setFrameStyle(TQFrame::StyledPanel | TQFrame::Sunken);
	setLineWidth(5);
}

BigLabel::~BigLabel()
{
}

void BigLabel::setTip(const TQString& text)
{
	TQToolTip::remove(this);
	TQToolTip::add(this, text);
}

#include "biglabel.moc"
