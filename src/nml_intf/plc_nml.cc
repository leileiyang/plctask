/*
*	New C++ File starts here.
*	This file should be named plc_nml.cc
*	Automatically generated by NML CodeGen Java Applet.
*	with command line arguments :  HHFile=plc_nml.hh -o plc_nml.cc
*
*	.gen script :
*		0:load plc_nml.hh
*		1:clear
*		2:select_from_file plc_nml.hh
*		3:generate C++ format>plc_nml.cc
*		4:generate C++ update>plc_nml.cc
*		5:generate C++ constructor>plc_nml.cc
*		6:exit
*
*/

// Include all NML, CMS, and RCS classes and functions
#include "rcs.hh"

// Include command and status message definitions
#include "plc_nml.hh"
#include "plc.hh"

// Forward Function Prototypes


#ifndef PLCNAME_LIST_LENGTH
#define PLCNAME_LIST_LENGTH 2
#endif

const NMLTYPE plcid_list[PLCNAME_LIST_LENGTH]= {
	PLC_STAT_TYPE, /* 0,101 */
	-1};
const size_t plcsize_list[PLCNAME_LIST_LENGTH]= {
	sizeof(PLC_STAT),
	0};


// Enumerated Type Constants

/*
Estimated_size	PLC_STAT	128
Estimated_size	PLC_STAT_MSG	128
Estimated_size	MAXIMUM	128
*/
/*
*	NML/CMS Format function : plcFormat
*/
int plcFormat(NMLTYPE type, void *buffer, CMS *cms)
{

	type = cms->check_type_info(type,buffer,"pl",
		(cms_symbol_lookup_function_t) 0,
		(const char **) 0,
		plcid_list,plcsize_list,
		PLCNAME_LIST_LENGTH,
		0);

	switch(type)
	{
	case PLC_STAT_TYPE:
		((PLC_STAT *) buffer)->update(cms);
		break;

	default:
		return(0);
	}
	return 1;
}

/*
*	NML/CMS Update function for PLC_STAT
*	from plc_nml.hh:12
*/
void PLC_STAT::update(CMS *cms)
{

	cms->beginClass("PLC_STAT","PLC_STAT_MSG");

	cms->beginBaseClass("PLC_STAT_MSG");
	PLC_STAT_MSG::update(cms);
	cms->endBaseClass("PLC_STAT_MSG");


	cms->endClass("PLC_STAT","PLC_STAT_MSG");

}


/*
*	NML/CMS Update function for PLC_STAT_MSG
*	from plc_nml.hh:5
*/
void PLC_STAT_MSG::update(CMS *cms)
{

	cms->beginClass("PLC_STAT_MSG","RCS_STAT_MSG");

	RCS_STAT_MSG::update_stat_msg_base(cms);

	cms->endClass("PLC_STAT_MSG","RCS_STAT_MSG");

}

/*
*	Constructor for PLC_STAT
*	from plc_nml.hh:12
*/
PLC_STAT::PLC_STAT()
	: PLC_STAT_MSG(PLC_STAT_TYPE,sizeof(PLC_STAT))
{

}

