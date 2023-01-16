#include "myplugincontroller.h"
#include "myplugincids.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ustring.h"

using namespace Steinberg;

namespace DCOffsetter {

//------------------------------------------------------------------------
// DCOffsetParameter Declaration (overwriting toString and fromString)
//------------------------------------------------------------------------
class DCOffsetParameter : public Vst::RangeParameter
{
public:
	DCOffsetParameter(int32 flags, int32 id);

	void toString(Vst::ParamValue normValue, Vst::String128 string) const SMTG_OVERRIDE;
	bool fromString(const Vst::TChar* string, Vst::ParamValue& normValue) const SMTG_OVERRIDE;
};

//------------------------------------------------------------------------
// DCOffsetParameter Implementation
//------------------------------------------------------------------------
DCOffsetParameter::DCOffsetParameter(int32 flags, int32 id)
{
	Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("DC Offset"));

	info.flags = flags;
	info.id = id;
	info.stepCount = 0;
	info.defaultNormalizedValue = 0.5f;
	info.unitId = Vst::kRootUnitId;
}

//------------------------------------------------------------------------
void DCOffsetParameter::toString(Vst::ParamValue normValue, Vst::String128 string) const
{
	char text[32];
	snprintf(text, 32, "%.2f", normValue * 2 - 1);
	Steinberg::UString(string, 128).fromAscii(text);
}

//------------------------------------------------------------------------
bool DCOffsetParameter::fromString(const Vst::TChar* string, Vst::ParamValue& normValue) const
{
	String wrapper((Vst::TChar*)string); // Don't know buffer size here!
	double tmp = 0.0;
	if (wrapper.scanFloat(tmp))
	{
		if (tmp > 1.0)
			normValue = 1.0;
		else if (tmp < -1.0)
			normValue = -1.0;
		else
			normValue = (tmp + 1) / 2;
		return true;
	}

	return false;
}

//------------------------------------------------------------------------
// DCOffsetterController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterController::initialize (FUnknown* context)
{
	// Here the plugin will be instantiated
	// Do not forget to call parent (if everything is ok, continue)
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
		return result;

	// Register parameters
	setKnobMode(Vst::kLinearMode);
	auto* dcOffsetParam = new DCOffsetParameter(Vst::ParameterInfo::kCanAutomate, MyParams::kParamDCOffsetId);
	parameters.addParameter(dcOffsetParam);
	parameters.addParameter(STR16("Clip"), 0, 1, 0.0, Vst::ParameterInfo::kCanAutomate, MyParams::kParamClipId, 0);
	parameters.addParameter(STR16("2x Multiplier"), 0, 1, 0.0, Vst::ParameterInfo::kCanAutomate, MyParams::kParamMultiplierId, 0);

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterController::terminate ()
{
	// Here the plugin will be de-instantiated, last possibility to remove some memory!
	// Do not forget to call parent
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterController::setComponentState (IBStream* state)
{
	// Here you get the state of the component (processor part)
	if (!state)
		return kResultFalse;

	IBStreamer streamer(state, kLittleEndian);
	float savedParam;

	if (streamer.readFloat(savedParam) == false)
		return kResultFalse;
	if (auto param = parameters.getParameter(MyParams::kParamDCOffsetId))
		param->setNormalized(savedParam);

	if (streamer.readFloat(savedParam) == false)
		return kResultFalse;
	if (auto param = parameters.getParameter(MyParams::kParamClipId))
		param->setNormalized(savedParam);

	if (streamer.readFloat(savedParam) == false)
		return kResultFalse;
	if (auto param = parameters.getParameter(MyParams::kParamMultiplierId))
		param->setNormalized(savedParam);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterController::setState (IBStream* state)
{
	// Here you get the state of the controller
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterController::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of the plugin is saved in the processor
	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API DCOffsetterController::createView (FIDString name)
{
	// Here the host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// Create the editor here and return an IPlugView ptr of it
		auto* view = new VSTGUI::VST3Editor (this, "view", "myplugineditor.uidesc");
		return view;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterController::setParamNormalized (Vst::ParamID tag, Vst::ParamValue value)
{
	// Called by host to update your parameters
	tresult result = EditControllerEx1::setParamNormalized (tag, value);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterController::getParamStringByValue (Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
{
	// Called by host to get a string for a given normalized value of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API DCOffsetterController::getParamValueByString (Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
{
	// Called by host to get a normalized value from a string representation of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}

//------------------------------------------------------------------------
}
