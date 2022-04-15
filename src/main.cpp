#include <tdeapplication.h>
#include <tdecmdlineargs.h>
#include <tdeaboutdata.h>

#include <ksimpleconfig.h>

#include "easyprojector.h"

int main( int argc, char *argv[] )
{
	TDEAboutData about(
		"tdeeasyprojector", "TDE Easy Projector", "0.1",
		"Easy projector setup utility",
		TDEAboutData::License_GPL,
		"Copyright (c) 2022 Mavridis Philippe (blu.256)"
	);

	TDECmdLineArgs::init(argc, argv, &about);
	TDEApplication app;

	KSimpleConfig cfg("easyprojectorrc");
	bool horizontalLayout = cfg.readBoolEntry("HorizontalLayout", false);

	EasyProjector *easy = new EasyProjector(horizontalLayout);
	app.setMainWidget(easy);
	easy->show();
	return app.exec();
}
