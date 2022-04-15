#include <tdeapplication.h>
#include <kiconloader.h>

#include <tqpushbutton.h>
#include <tqlayout.h>
#include <tqlabel.h>

#include <tdelocale.h>

#include "dialog.h"

ExtendedViewPosition::ExtendedViewPosition(TQWidget *parent)
	: KDialogBase( i18n("Select side to extend to"), Cancel, NoDefault, Cancel, parent )
{
	TQWidget     *main = new TQWidget(this);
	TQGridLayout *grid = new TQGridLayout(main, 3, 3);

	TQLabel	*screenPic = new TQLabel(main);
	screenPic->setPixmap(kapp->iconLoader()->loadIcon("projection-pc-only", TDEIcon::Desktop, TDEIcon::SizeLarge));
	screenPic->setAlignment(TQt::AlignCenter);
	grid->addWidget(screenPic, 1, 1);

	TQPushButton *bLeft  = new TQPushButton(main);
	TQPushButton *bRight = new TQPushButton(main);
	TQPushButton *bUp    = new TQPushButton(main);
	TQPushButton *bDown  = new TQPushButton(main);

	bLeft ->setPixmap(kapp->iconLoader()->loadIcon("1leftarrow",  TDEIcon::Small));
	bRight->setPixmap(kapp->iconLoader()->loadIcon("1rightarrow", TDEIcon::Small));
	bUp   ->setPixmap(kapp->iconLoader()->loadIcon("1uparrow",    TDEIcon::Small));
	bDown ->setPixmap(kapp->iconLoader()->loadIcon("1downarrow",  TDEIcon::Small));

	int size = screenPic->width();
	bLeft ->setMinimumSize(size, size);
	bRight->setMinimumSize(size, size);
	bUp   ->setMinimumSize(size, size);
	bDown ->setMinimumSize(size, size);

	grid->addWidget(bLeft,  1, 0);
	grid->addWidget(bRight, 1, 2);
	grid->addWidget(bUp,    0, 1);
	grid->addWidget(bDown,  2, 1);

	connect(bLeft,  SIGNAL(clicked()), this, SLOT(selectLeft()));
	connect(bRight, SIGNAL(clicked()), this, SLOT(selectRight()));
	connect(bUp,    SIGNAL(clicked()), this, SLOT(selectUp()));
	connect(bDown,  SIGNAL(clicked()), this, SLOT(selectDown()));

	setMainWidget(main);
	enableButtonSeparator(true);
}

ExtendedViewPosition::~ExtendedViewPosition()
{
}

void ExtendedViewPosition::selectLeft()
{
	m_side = Left;
	accept();
}

void ExtendedViewPosition::selectRight()
{
	m_side = Right;
	accept();
}

void ExtendedViewPosition::selectUp()
{
	m_side = Up;
	accept();
}

void ExtendedViewPosition::selectDown()
{
	m_side = Down;
	accept();
}

#include "dialog.moc"
