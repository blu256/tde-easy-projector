#include <tdemessagebox.h>
#include <tdelocale.h>
#include <kprocess.h>

#include <kdebug.h>

#include "bigbutton.h"
#include "biglabel.h"
#include "dialog.h"

#include "easyprojector.h"


EasyProjector::EasyProjector(bool horizontal)
	: TQHBox(horizontal, 0, "tdeeasyprojector")
{
	setCaption(i18n("TDE Easy Projector"));

	m_state = new BigLabel(i18n("Loading..."), this);
	m_simplerandr = new KRandrSimpleAPI();

	m_pc_only   = new BigButton(this, "projection-pc-only",   i18n("PC Screen only"),              this, SLOT(togglePCOnly(bool)));
	m_duplicate = new BigButton(this, "projection-duplicate", i18n("Projector duplicates screen"), this, SLOT(toggleDuplicate(bool))); 
	m_extend    = new BigButton(this, "projection-extend",    i18n("Projector extends screen"),    this, SLOT(toggleExtend(bool)));

	m_pc_only->setTip(i18n("Disable projector output."));
	m_duplicate->setTip(i18n("Projector shows a copy of your screen."));
	m_extend->setTip(i18n("Projector acts as an extension of your screen (either of the four sides)."));

	// Check randr support
	if(!m_simplerandr->kRandrHasRandr())
	{
		setButtonsState(false);
		m_state->setText(i18n("System not supported."));
		m_state->setTip(i18n("This system does not support the XRandR extension."));
	}
	else
	{
		// Resort to single screen mode
		updateState();
		m_pc_only->setOn(true);
	}
}

EasyProjector::~EasyProjector()
{
	delete m_state;
}

void EasyProjector::setButtonsState(bool on)
{
	m_pc_only->setEnabled(on);
	m_duplicate->setEnabled(on);
	m_extend->setEnabled(on);
}

void EasyProjector::updateState()
{
	m_screens = m_simplerandr->readCurrentDisplayConfiguration();
	int screens = 0;

	SingleScreenData *screen;
	for( screen = m_screens.first(); screen; screen = m_screens.next() )
	{
		if(screen->screen_connected)
		{
			++screens;
		}
	}

	if(!screens)
	{
		setButtonsState(false);

		m_state->setText(i18n("No screens found."));
		m_state->setTip(i18n("No outputs have been found on this computer. This probably means that there is a system error."));
		return;
	}
	else if( screens == 1 )
	{
		setButtonsState(false);
		m_pc_only->setOn(true);

		m_state->setText(i18n("No projector."));
		m_state->setTip(i18n("Ensure that the projector is properly functioning and connected to this computer."));
		return;
	}

	for( screen = m_screens.first(); screen; screen = m_screens.next() )
	{
		if( screen->is_primary )
		{
			m_primary = screen;
		}
		else
		{
			m_projector = screen;
		}
	}

	// Consider HDMI to be projector by default
	if(m_primary->screenUniqueName.contains("HDMI"))
	{
		SingleScreenData *tmp = m_primary;
		m_primary = m_projector;
		m_projector = tmp;
	}

	if(!m_primary || !m_projector)
	{
		kdDebug() << "primary: " << (bool)m_primary << ", projector: " << (bool)m_projector << endl;
		m_state->setText(i18n("Internal error"));
		m_state->setTip(i18n("Either the primary screen or the projector could not be found."));
		return;
	}

	m_state->setTip(i18n("<table><tr><td><b>Screen:</b></td><td>%1</td></tr><tr><td><b>Projector:</b></td><td>%2</td></tr></table>")
			.arg(m_primary->screenFriendlyName)
			.arg(m_projector->screenFriendlyName));

	switch(m_mode)
	{
		case PC_Only:
			m_state->setText(i18n("PC Only"));
			break;

		case Duplicate:
			m_state->setText(i18n("Duplicated"));
			break;

		case Extend:
		{
			switch(m_side)
			{
				case ExtendedViewPosition::Left:
					m_state->setText(i18n("Extending on the left"));
					break;

				case ExtendedViewPosition::Right:
					m_state->setText(i18n("Extending on the right"));
					break;

				case ExtendedViewPosition::Up:
					m_state->setText(i18n("Extending on the top"));
					break;

				case ExtendedViewPosition::Down:
					m_state->setText(i18n("Extending on the bottom"));
					break;
			}
		}
	}
}

TQString randrName(SingleScreenData *screen)
{
	TQStringList s = TQStringList::split(":", screen->screenUniqueName, true);
	if(s.count() != 3)
	{
		return TQString::null;
	}

	TQString name = s[2];

	// capitalization is wrong if it's eDP
	name.replace("EDP", "eDP");	

	return name;
}

TQString EasyProjector::bestProjectorResolution()
{
	// Try to match current resolution
	TQString screenResolution = m_primary->resolutions[m_primary->current_resolution_index];
	TQStringList projectorResolutions = m_projector->resolutions;
	TQStringList::iterator it;
	for(it = projectorResolutions.begin(); it != projectorResolutions.end(); ++it)
	{
		if((*it) == screenResolution)
		{
			return (*it).replace(" ", "");
		}
	}

	return projectorResolutions[0].replace(" ", "");
}

static bool switchingState = false;

void EasyProjector::togglePCOnly(bool on)
{
	if(!m_projector || switchingState) return;

	if(!on && m_mode == PC_Only)
	{
		m_pc_only->setOn(true);
		return;
	}

	m_mode = PC_Only;

	switchingState = true;
	m_duplicate->setOn(false);
	m_extend->setOn(false);
	switchingState = false;

	// BUGGY
	// m_projector->is_primary = false;
	// m_projector->is_extended = false;
	// m_simplerandr->applyDisplayConfiguration(m_screens, false);
	
	TDEProcess *proc = new TDEProcess;
	*proc << "xrandr";
	*proc << "--output" << randrName(m_projector);
	*proc << "--off";
	proc->start();
	
	connect(proc, SIGNAL(receivedStderr(TDEProcess*, char*, int)), this, SLOT(receivedStderr(TDEProcess*, char*, int)));

	updateState();
}

void EasyProjector::toggleDuplicate(bool on)
{
	if(!m_projector || switchingState) return;

	if(!on && m_mode == Duplicate)
	{
		m_duplicate->setOn(true);
		return;
	}

	m_mode = Duplicate;

	switchingState = true;
	m_pc_only->setOn(false);
	m_extend->setOn(false);
	switchingState = false;

	TDEProcess *proc = new TDEProcess;
	*proc << "xrandr";
	*proc << "--output" << randrName(m_projector);
	*proc << "--mode" << bestProjectorResolution();
	*proc << "--same-as" << randrName(m_primary);
	proc->start();
	
	connect(proc, SIGNAL(receivedStderr(TDEProcess*, char*, int)), this, SLOT(receivedStderr(TDEProcess*, char*, int)));

	updateState();
}

void EasyProjector::toggleExtend(bool on)
{
	if(!m_projector || switchingState) return;

	if(!on && m_mode == Extend)
	{
		m_extend->setOn(true);
		return;
	}

	m_mode = Extend;

	switchingState = true;
  	m_pc_only->setOn(false);
	m_duplicate->setOn(false);
	switchingState = false;

	// determine relative position
	ExtendedViewPosition dlg(this);
	if( dlg.exec() != TQDialog::Accepted )
	{
		m_pc_only->setOn(true);
		return;
	}

	m_side = dlg.side();

	TQString relative_to = TQString::null;
	switch(m_side)
	{
		case ExtendedViewPosition::Left:
			relative_to = "--left-of";
			break;
		
		case ExtendedViewPosition::Right:
			relative_to = "--right-of";
			break;

		case ExtendedViewPosition::Up:
			relative_to = "--above";
			break;

		case ExtendedViewPosition::Down:
			relative_to = "--below";
			break;
	}

	if(relative_to.isNull())
	{
		m_pc_only->setOn(false);
		return;
	}

	TDEProcess *proc = new TDEProcess;
	*proc << "xrandr";
	*proc << "--output" << randrName(m_projector);
	*proc << "--mode" << bestProjectorResolution();
	*proc << relative_to << randrName(m_primary);
	proc->start();
	
	connect(proc, SIGNAL(receivedStderr(TDEProcess*, char*, int)), this, SLOT(receivedStderr(TDEProcess*, char*, int)));

	updateState();
}

void EasyProjector::receivedStderr(TDEProcess *proc, char *buffer, int bufflen)
{
	KMessageBox::sorry(this, i18n("Could not change display settings!"), i18n("Error"));
}

#include "easyprojector.moc"
