#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <kdialogbase.h>

class ExtendedViewPosition : public KDialogBase
{
	TQ_OBJECT
	
	public:
		ExtendedViewPosition(TQWidget *parent);
		~ExtendedViewPosition();

		enum Side { Left, Right, Up, Down };

		Side side() { return m_side; }

	private:
		Side m_side;

	private slots:
		void selectLeft();
		void selectRight();
		void selectUp();
		void selectDown();
};

#endif // __DIALOG_H__
