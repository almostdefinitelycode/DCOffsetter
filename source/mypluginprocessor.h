#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace JonasM {

//------------------------------------------------------------------------
//  DCOffsetterProcessor
//------------------------------------------------------------------------
class DCOffsetterProcessor : public Steinberg::Vst::AudioEffect
{
public:
	DCOffsetterProcessor ();
	~DCOffsetterProcessor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{ 
		return (Steinberg::Vst::IAudioProcessor*)new DCOffsetterProcessor; 
	}

	//------------------------------------------------------------------------
	// AudioEffect overrides:
	//------------------------------------------------------------------------
	/** Called after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	
	/** Called before destructor */
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	
	/** Switch the plugin on/off */
	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	/** Called before any process call */
	Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
	
	/** Asks if a given sample size is supported (see SymbolicSampleSizes) */
	Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	/** Here we go...the process call */
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

//------------------------------------------------------------------------
protected:
	double mDCOffset = 0.5;
	double mClip = 0.0;
	double mMultiplier = 0.0;

};

//------------------------------------------------------------------------
}
