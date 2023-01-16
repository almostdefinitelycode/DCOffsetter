#include "mypluginprocessor.h"
#include "myplugincontroller.h"
#include "myplugincids.h"
#include "version.h"
#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "DCOffsetter"

using namespace Steinberg::Vst;
using namespace JonasM;

//------------------------------------------------------------------------
//  VST Plugin Entry
//------------------------------------------------------------------------

BEGIN_FACTORY_DEF ("Jonas M", 
			       "https://www.mycompanyname.com", 
			       "mailto:info@mycompanyname.com")

	//---First plugin included in this factory---
	// its kVstAudioEffectClass component
	DEF_CLASS2 (INLINE_UID_FROM_FUID(kDCOffsetterProcessorUID),
				PClassInfo::kManyInstances, // cardinality
				kVstAudioEffectClass, // the component category (do not changed this)
				stringPluginName, // here the plugin name (to be changed)
				Vst::kDistributable, // means that component and controller could be distributed on different computers
				DCOffsetterVST3Category, // Subcategory for this plugin (to be changed)
				FULL_VERSION_STR, // Plugin version (to be changed)
				kVstVersionString, // the VST 3 SDK version (do not changed this, use always this define)
				DCOffsetterProcessor::createInstance) // function pointer called when this component should be instantiated

	// its kVstComponentControllerClass component
	DEF_CLASS2 (INLINE_UID_FROM_FUID (kDCOffsetterControllerUID),
				PClassInfo::kManyInstances, // cardinality
				kVstComponentControllerClass, // the Controller category (do not changed this)
				stringPluginName "Controller", // controller name (could be the same than component name)
				0, // not used here
				"", // not used here
				FULL_VERSION_STR, // Plugin version (to be changed)
				kVstVersionString, // the VST 3 SDK version (do not changed this, use always this define)
				DCOffsetterController::createInstance) // function pointer called when this component should be instantiated

END_FACTORY
