All the FX and Amp units in ToneStack are conveniently MIDI learnable. The collection of MIDI assignments for a unit is called a "map", as it maps controls on your MIDI hardware to particular knobs or switches in ToneStack.

To start out, you 'teach' ToneStack MIDI by a feature called "MIDI Learn". You can access this feature by going to App Settings -> MIDI and turning on the MIDI Learn (Units) switch. After that, all you have to do is select the ToneStack parameter you want to map to and then manipulate an element on your MIDI controller that you intend to use. If you have an iPhone, double tap a unit icon on the Signal Chain screen to pull up its MIDI learn interface. If your MIDI device is connected properly, you should immediately see a MIDI CC number, as well as a channel ID on that ToneStack Parameter. When finished, just tap DONE on the upper right (iPad) or turn off MIDI learn via the switch you turned it on with (both iPhone and iPad).

To access advanced MIDI learn features, double tap a control element on the Units Control Screen. This will pull up a dialog where you can manually enter CC/Channel information. You can also specify minimum and maximum MIDI values for a continuous paramater: this is useful in situations where you want to use the full range of your physical MIDI controller, but want to control a narrow interval of values in a ToneStack parameter.

Continuous ToneStack parameters also provide an optional STATE CONTROL feature. This feature is convenient in situation where you want to be able to control a parameter and toggle the unit on or off using the same CC. With STATE CONTROL turned on, an incoming MIDI value of 0 will turn off the unit. When MIDI value is increased, the unit will automatically turn back on. This is particularly helpful when using expression pedals with units like wahs and pitch mods: when the pedal is in fully heel-down position, the unit will turn off. To turn unit back on, simply depress the pedal slightly.

For binary parameters (i.e., toggle and switches), you can also choose between LATCH or MOMENTARY control types. In LATCH mode, the MIDI controller is assumed to send two different values for the on and off states. In MOMENTARY mode, the MIDI controller is assumed to send two different values on button-down and button-up. In either case, the messages are handled to work correctly with toggles and switches in ToneStack.

Each unit can have a default MIDI map that loads whenever you add it via the FX Browser. To save a default map, simply press "Unit Default: SAVE" after programming your unit, ensuring that one of the unit's MIDI Learn controls is selected (it will be red). To load the default map to a unit, select one of its parameters and then touch "Unit Default: LOAD".

You also have options for loading MIDI maps you created in the past. Every ToneStack preset saves the MIDI mapping data you programmed for it. If you want to load the MIDI mapping data in that preset whenever you load the preset, leave the "Load CC Map From Patch" switch under MIDI setting in the on position. If you want the units to inherit from the 'Default Map' we talked about above, put this switch in the off position.

You can separately specify what happens when you add a new unit via the FX Browser. If the "Load Unit Default on Add" switch is on, each new unit that you add will inherit its MIDI assignments from the default map. In the off position, each unit will instantiate with a clear map -- i.e., no assignments.

ToneStack also allows you to control certain app assets with MIDI. These include tuner, metronome, and audio levels controls, as well as sequential folder and preset selection. To learn these controls, touch MIDI Learn (App Controls) under MIDI settings menu.

In addition to up/down program changes, you are able to use standard MIDI program change messages to select a ToneStack preset, or use program change messages to select pads in an onSTAGE performance group. You can activate MIDI program changes by going to the MIDI settings menu. You can also specify which channel you would like to receive program change messages on.

To increase versatility, ToneStack has separate MIDI In on/off stiches for Phsyical/Networked and Virtual MIDI connections. For example, with Virtual MIDI In off, ToneStack will ignore MIDI messages from other virtual MIDI apps, but will still recognize phsyical or networked MIDI messages.

Go to MIDI DEVICES menu under MIDI SETTINGS to specify whether you wish to receive MIDI or clock messages from each individual MIDI device or network. These preferences are retained so they could be restored the next time the same device is connected.

To use ToneStack with a physical MIDI clock, or a MIDI clock from another app, put the Use Internal Clock switch on the MIDI Settings menu in the off position.
