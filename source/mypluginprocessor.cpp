#include "mypluginprocessor.h"
#include "myplugincids.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;

namespace DCOffsetter {
//------------------------------------------------------------------------
// DCOffsetterProcessor
//------------------------------------------------------------------------
DCOffsetterProcessor::DCOffsetterProcessor ()
{
	// Set the desired controller for your processor
	setControllerClass (kDCOffsetterControllerUID);
}

//------------------------------------------------------------------------
DCOffsetterProcessor::~DCOffsetterProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterProcessor::initialize (FUnknown* context)
{
	// Here the plugin will be instantiated
	// Always initialize parent (if everything is ok, continue)
	tresult result = AudioEffect::initialize (context);
	if (result != kResultOk)
		return result;

	// Create audio IO
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	// If you don't need an event bus, you can remove the next line
	// addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterProcessor::terminate ()
{
	// Here the plugin will be de-instantiated, last possibility to remove some memory!
	// Do not forget to call parent
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterProcessor::setActive (TBool state)
{
	// Called when the plugin is enabled/disabled
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterProcessor::process (Vst::ProcessData& data)
{
	// Read input parameter changes
	if (data.inputParameterChanges)
	{
		int32_t numParamsChanged = data.inputParameterChanges->getParameterCount();
		for (int32_t i = 0; i < numParamsChanged; i++)
		{
			Vst::IParamValueQueue* paramQueue = data.inputParameterChanges->getParameterData(i);
			if (paramQueue)
			{
				Vst::ParamValue value;
				int32_t sampleOffset;
				int32_t numPoints = paramQueue->getPointCount();
				switch (paramQueue->getParameterId())
				{
				case MyParams::kParamDCOffsetId:
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						mDCOffset = value;
					break;

				case MyParams::kParamClipId:
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						mClip = value;
					break;

				case MyParams::kParamMultiplierId:
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						mMultiplier = value;
					break;
				}
			}
		}
	}

	// Check for input
	if (data.numInputs == 0 || data.numSamples == 0)
		return kResultOk;

	// Get audio buffers using helper-functions (vstaudioprocessoralgo.h)
	uint32_t sampleFramesSize = getSampleFramesSizeInBytes(processSetup, data.numSamples);
	void** in = getChannelBuffersPointer(processSetup, data.inputs[0]);
	void** out = getChannelBuffersPointer(processSetup, data.outputs[0]);

	// Mark outputs as not silent
	data.outputs[0].silenceFlags = 0;

	// Process each channel (stereo: left and right)
	int32_t numChannels = data.inputs[0].numChannels;
	for (int32_t i = 0; i < numChannels; i++)
	{
		int32_t samples = data.numSamples;
		float* ptrIn = (float*)in[i];
		float* ptrOut = (float*)out[i];
		float tmp;

		// For each sample in this channel
		while (--samples >= 0)
		{
			// Apply DC offset
			if (mMultiplier)
				tmp = (*ptrIn + mDCOffset * 4 - 2);
			else
				tmp = (*ptrIn + mDCOffset * 2 - 1);

			// Apply clipping
			if (mClip)
				tmp = std::max(-1.0f, std::min(tmp, 1.0f));

			*ptrOut = tmp;
			ptrIn++;
			ptrOut++;
		}
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	// Called before any processing
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// By default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// Disable the following comment if your processing supports kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterProcessor::setState (IBStream* state)
{
	// Called when a preset or project is loaded
	IBStreamer streamer(state, kLittleEndian);

	float savedParam;
	if (streamer.readFloat(savedParam) == false)
		return kResultFalse;
	mDCOffset = savedParam;

	if (streamer.readFloat(savedParam) == false)
		return kResultFalse;
	mClip = savedParam;

	if (streamer.readFloat(savedParam) == false)
		return kResultFalse;
	mMultiplier = savedParam;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterProcessor::getState (IBStream* state)
{
	// Called when a preset or project is saved
	IBStreamer streamer(state, kLittleEndian);
	streamer.writeFloat(mDCOffset);
	streamer.writeFloat(mClip);
	streamer.writeFloat(mMultiplier);

	return kResultOk;
}

//------------------------------------------------------------------------
}
