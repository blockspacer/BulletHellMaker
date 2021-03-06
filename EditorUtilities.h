#pragma once
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include "SpriteLoader.h"
#include "Animatable.h"
#include "Animation.h"
#include "AudioPlayer.h"
#include "TimeFunctionVariable.h"
#include "UndoStack.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "EnemySystem.h"
#include "DespawnSystem.h"
#include "SpriteAnimationSystem.h"
#include "EntityCreationQueue.h"
#include "ShadowTrailSystem.h"
#include "PlayerSystem.h"
#include "AudioPlayer.h"
#include "CollectibleSystem.h"
#include "DebugRenderSystem.h"
#include "RenderSystem.h"
#include "Attack.h"
#include "EditorMovablePointAction.h"
#include "EditorMovablePoint.h"
#include "MovablePoint.h"
#include "ViewController.h"
#include "EventCapturable.h"
#include "LRUCache.h"
#include "ExtraSignals.h"
#include <memory>
#include <thread>
#include <mutex>
#include <entt/entt.hpp>
#include <tuple>

/*
Sends window to the foreground of the computer display.
*/
void sendToForeground(sf::RenderWindow& window);
/*
Returns a tooltip containing some text.
*/
std::shared_ptr<tgui::Label> createToolTip(std::string text);
/*
Returns a menu popup with clickable buttons. The height of the popup will always be the exact height needed to fit
all menu items without needing a scrollbar.

elements - a vector of pairs containing the text of the button and the function to be called when the button is pressed
*/
std::shared_ptr<tgui::ListBox> createMenuPopup(std::vector<std::pair<std::string, std::function<void()>>> elements);
/*
Returns a sf::VertexArray that contains the positions that an entity following the array of EMPAs will be in over time.
Note that there must be an entity with the PlayerTag component if any of the EMPAs or their EMPAAngleOffsets require the player's position.
The vertices linearly interpolate from startColor to endColor as the path progresses.

timeResolution - the amount of time between each point
x, y - the starting position of the returned vertex array
playerX, playerY - position of the player
*/
sf::VertexArray generateVertexArray(std::vector<std::shared_ptr<EMPAction>> actions, float timeResolution, float x, float y, float playerX, float playerY, sf::Color startColor = sf::Color::Red, sf::Color endColor = sf::Color::Blue);
sf::VertexArray generateVertexArray(std::shared_ptr<EMPAction> action, float timeResolution, float x, float y, float playerX, float playerY, sf::Color startColor = sf::Color::Red, sf::Color endColor = sf::Color::Blue);
/*
Returns an array of segment data. Each segment data is 2 vectors of floats in order:
the x-coordinates and the y-coordinates of a matplotlibc curve.
The x-axis represents time in seconds and the y-axis the TFV's value.

timeResolution - the amount of time between each point
colors - the list of colors that will be looped to determine the color of each segment of the curve
*/
std::vector<std::pair<std::vector<float>, std::vector<float>>> generateMPLPoints(std::shared_ptr<PiecewiseTFV> tfv, float tfvLifespan, float timeResolution);

class EditorWindow;
class UndoableEditorWindow;

/*
A tgui::Group whose size will be (0, 0) while invisible.
*/
class HideableGroup : public tgui::Group {
public:
	const tgui::Layout2d& getSizeLayout() const;

	void setSize(const tgui::Layout2d& size) override;
	void setSize(tgui::Layout width, tgui::Layout height);
	void setVisible(bool visible) override;

private:
	// The size of this widget right before setVisible(false) is called
	tgui::Layout2d savedSize;
};

/*
A tgui::Slider that allows for values not limited by multiples of the slider's step.
*/
class Slider : public tgui::Slider {
public:
	void setValue(float value);
	using tgui::Slider::setValue;

	void setStep(float step);
	using tgui::Slider::setStep;
};

/*
A Slider that waits for some time after its value stops changing before actually emitting
the ValueChanged signal.

Signals:
	ValueChanged - emitted VALUE_CHANGE_WINDOW seconds after the slider value is changed.
		Optional parameter: the new value of the slider as a float
*/
class DelayedSlider : public Slider {
public:
	DelayedSlider();
	static std::shared_ptr<DelayedSlider> create() {
		return std::make_shared<DelayedSlider>();
	}

	void update(sf::Time elapsedTime) override;
	tgui::Signal& getSignal(std::string signalName) override;

private:
	// The amount of time that must pass since the last slider value change before
	// the onValueChange signal is emitted
	const float VALUE_CHANGE_WINDOW = 0.2f;

	// Emitted VALUE_CHANGE_WINDOW seconds after the slider value is changed
	tgui::SignalFloat onValueChange = {"ValueChanged"};
	// The amount of time that has elapsed in seconds since the last slider value change
	float timeElapsedSinceLastValueChange = 0;
	// Whether the onValueChange signal has been emitted since the last slider value change
	bool valueChangeSignalEmitted = false;

	// Used for a small hack to have getSignal() return tgui::Slider's onValueChange
	// signal rather than DelayedSlider's onValueChange when this bool is true
	bool ignoreDelayedSliderSignalName;
};

/*
An edit box that accepts only numbers and can have upper/lower limits.

Signals:
	ValueChanged - emitted when the edit box value is changed, either when the enter
		key is pressed or the widget is unfocused.
		Optional parameter: the value in the edit box as a float. If this edit box is
		in integer mode, the value can just be cast to an int.
*/
class NumericalEditBoxWithLimits : public tgui::EditBox {
public:
	NumericalEditBoxWithLimits();
	static std::shared_ptr<NumericalEditBoxWithLimits> create() {
		return std::make_shared<NumericalEditBoxWithLimits>();
	}

	float getValue();
	void setValue(int value);
	void setValue(float value);
	/*
	Sets the min value of the edit box.
	*/
	void setMin(float min);
	/*
	Sets the max value of the edit box.
	*/
	void setMax(float max);
	/*
	Removes the min value limit on the edit box.
	*/
	void removeMin();
	/*
	Removes the max value limit on the edit box.
	*/
	void removeMax();
	/*
	Makes the edit box accept only integers.
	This is false by default.
	*/
	void setIntegerMode(bool intMode);

	inline bool getHasMin() { return hasMin; }
	inline bool getHasMax() { return hasMax; }

	tgui::Signal& getSignal(std::string signalName) override;

private:
	void updateInputValidator();

	// Emitted when the edit box value is changed
	tgui::SignalFloat onValueChange = { "ValueChanged" };

	bool hasMin = false, hasMax = false;
	float min, max;
	// if true, the inputted number must be an integer
	bool mustBeInt = false;

	// bool used to ignore signals to prevent infinite loops
	bool ignoreSignals = false;
};

/*
A Group containing a DelayedSlider whose value can be set with a 
NumericalEditBoxWithLimits located on its right.

ValueChanged - emitted VALUE_CHANGE_WINDOW seconds after the slider value or edit box value is changed.
		Optional parameter: the new value as a float
*/
class SliderWithEditBox : public HideableGroup {
public:
	SliderWithEditBox();
	static std::shared_ptr<SliderWithEditBox> create() {
		return std::make_shared<SliderWithEditBox>();
	}

	float getValue();
	void setValue(int value);
	void setValue(float value);
	/*
	Sets the min value of the slider and edit box.
	*/
	void setMin(float min);
	/*
	Sets the max value of the slider and edit box.
	*/
	void setMax(float max);
	/*
	Sets the step of the slider.
	*/
	void setStep(float step);
	/*
	Whether the slider and edit box can accept only integers.
	*/
	void setIntegerMode(bool intMode);
	/*
	Sets the text size of the edit box.
	*/
	void setTextSize(int textSize);

	tgui::Signal& getSignal(std::string signalName) override;

private:
	std::shared_ptr<DelayedSlider> slider;
	std::shared_ptr<NumericalEditBoxWithLimits> editBox;

	tgui::SignalFloat onValueChange = { "ValueChanged" };

	float lastKnownValue;

	// bool used to ignore signals to prevent infinite loops
	bool ignoreSignals = false;
};

/*
A widget used to display Animatables.
*/
class AnimatablePicture : public tgui::Widget {
public:
	AnimatablePicture();
	AnimatablePicture(const AnimatablePicture& other);
	static std::shared_ptr<AnimatablePicture> create() {
		return std::make_shared<AnimatablePicture>();
	}

	void update(sf::Time elapsedTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual tgui::Widget::Ptr clone() const override;
	virtual bool mouseOnWidget(tgui::Vector2f pos) const override;

	void setAnimation(SpriteLoader& spriteLoader, const std::string& animationName, const std::string& spriteSheetName);
	void setSprite(SpriteLoader& spriteLoader, const std::string& spriteName, const std::string& spriteSheetName);

private:
	std::unique_ptr<Animation> animation;
	std::shared_ptr<sf::Sprite> curSprite;

	bool spriteScaledToFitHorizontal;

	void resizeCurSpriteToFitWidget();
};

/*
Widget for choosing an Animatable from the list of all Animatables in a SpriteLoader.

Signals:
	ValueChanged - emitted when a change is made to the Animatable object being edited
	Optional parameter: the new Animatable object

The height of this widget will always be just enough to fit all 
the widgets in it, so changing the height of this widget will do nothing.
*/
class AnimatableChooser : public HideableGroup {
public:
	/*
	forceSprite - if true, the user is forced to choose between sprites instead of being able to choose between sprites and animations
	*/
	AnimatableChooser(SpriteLoader& spriteLoader, bool forceSprite = false);
	static std::shared_ptr<AnimatableChooser> create(SpriteLoader& spriteLoader, bool forceSprite = false) {
		return std::make_shared<AnimatableChooser>(spriteLoader, forceSprite);
	}

	/*
	Sets the value of the Animatable being edited.
	*/
	void setValue(Animatable animatable);
	/*
	Returns an Animatable with empty sprite name and sprite sheet name if none is selected.
	*/
	Animatable getValue();

	void setEnabled(bool enabled);
	void setAnimatablePictureSize(tgui::Layout width, tgui::Layout height);
	void setAnimatablePictureSize(const tgui::Layout2d& size);

	tgui::Signal& getSignal(std::string signalName) override;

private:
	/*
	Calculates and sets the number of items to display, depending on this widget's position relative to its container.
	*/
	void calculateItemsToDisplay();

	SpriteLoader& spriteLoader;
	bool forceSprite;

	std::shared_ptr<tgui::ComboBox> animatable;
	std::shared_ptr<tgui::ComboBox> rotationType;

	std::shared_ptr<AnimatablePicture> animatablePicture;

	tgui::SignalAnimatable onValueChange = { "ValueChanged" };

	bool ignoreSignals = false;
};

/*
Used to edit a SoundSettings object.

Signals:
	ValueChanged - emitted when a change is made to the SoundSettings object being edited
	Optional parameter: the new SoundSettings object
*/
class SoundSettingsGroup : public HideableGroup {
public:
	SoundSettingsGroup(std::string pathToSoundsFolder);
	/*
	Initialize the widgets in this group to some SoundSettings's values
	*/
	void initSettings(SoundSettings init);
	/*
	Populate the file names list with all supported sound files in the directory
	*/
	void populateFileNames(std::string pathToSoundsFolder);

	void setEnabled(bool enabled);

	tgui::Signal& getSignal(std::string signalName) override;

private:
	// Emitted when a change is made to the SoundSettings object being edited
	tgui::SignalSoundSettings onValueChange = { "ValueChanged" };

	std::shared_ptr<tgui::CheckBox> enableAudio;
	std::shared_ptr<tgui::ComboBox> fileName;
	std::shared_ptr<SliderWithEditBox> volume;
	std::shared_ptr<SliderWithEditBox> pitch;

	std::shared_ptr<tgui::Label> enableAudioLabel;
	std::shared_ptr<tgui::Label> fileNameLabel;
	std::shared_ptr<tgui::Label> volumeLabel;
	std::shared_ptr<tgui::Label> pitchLabel;

	// bool used to ignore signals to prevent infinite loops
	bool ignoreSignals = false;
};


/*
A ScrollablePanel that can scroll horizontally as well as vertically and contains a ListBox.
The ListBox from getListBox() does not have to be added to a container, since it is already part of this ScrollablePanel.

This widget can be treated as a normal ListBox, with the exception that onListBoxItemsUpdate() should be called
anytime an item is added, removed, or changed from the ListBox.

The only advantage of a ListBox over a ListView is that items can be identified by a
unique id string. This can be easily implemented with your own std::map, however.
*/
class ListBoxScrollablePanel : public tgui::ScrollablePanel {
public:
	ListBoxScrollablePanel();
	static std::shared_ptr<ListBoxScrollablePanel> create() {
		return std::make_shared<ListBoxScrollablePanel>();
	}

	/*
	Should be called anytime an item is added, removed, or changed from the ListBox.
	*/
	void onListBoxItemsUpdate();
	bool mouseWheelScrolled(float delta, tgui::Vector2f pos) override;

	void setTextSize(int textSize);
	inline std::shared_ptr<tgui::ListBox> getListBox() { return listBox; }

private:
	std::shared_ptr<tgui::ListBox> listBox;
	std::shared_ptr<tgui::Label> textWidthChecker;
	float longestWidth;
};

/*
A ScrollablePanel that can scroll horizontally as well as vertically and contains a ListView.
The ListView from getListView() does not have to be added to a container, since it is already part of this ScrollablePanel.

This widget can be treated as a normal ListView, with the exception that onListViewItemsUpdate() should be called
anytime an item is added, removed, or changed from the ListView.

The only advantage of a ListBox over a ListView is that items can be identified by a
unique id string. This can be easily implemented with your own std::map, however.
*/
class ListViewScrollablePanel : public tgui::ScrollablePanel {
public:
	ListViewScrollablePanel();
	static std::shared_ptr<ListViewScrollablePanel> create() {
		return std::make_shared<ListViewScrollablePanel>();
	}

	/*
	Should be called anytime an item is added, removed, or has its text changed from the ListView.
	*/
	void onListViewItemsUpdate();
	bool mouseWheelScrolled(float delta, tgui::Vector2f pos) override;

	void setTextSize(int textSize);
	inline std::shared_ptr<tgui::ListView> getListView() { return listView; }

private:
	std::shared_ptr<tgui::ListView> listView;
	std::shared_ptr<tgui::Label> textWidthChecker;
	float longestWidth;
};

/*
Used to edit TFVs.

Signals:
	ValueChanged - emitted when a change is made to the TFV being edited.
		Optional parameter: a pair of a shared_ptr to the old TFV object 
			and shared_ptr to the new TFV object
*/
class TFVGroup : public HideableGroup {
public:
	TFVGroup(EditorWindow& parentWindow);
	static std::shared_ptr<TFVGroup> create(EditorWindow& parentWindow) {
		return std::make_shared<TFVGroup>(parentWindow);
	}

	/*
	Initialize this widget to tfv's values.
	tfv won't be modified by this widget.
	The old TFV emitted in the ValueChanged signal will tfv until the next setTFV() call.

	tfvLifespan - the max lifespan of tfv. If the TFV is in an EMPA, this value
		should be EMPA::getTime().
	*/
	void setTFV(std::shared_ptr<TFV> tfv, float tfvLifespan);

	tgui::Signal& getSignal(std::string signalName) override;

private:
	const float TFV_TIME_RESOLUTION = 0.05f; // Time between each tfv curve vertex

	EditorWindow& parentWindow;
	std::recursive_mutex tfvMutex;

	std::shared_ptr<tgui::Button> showGraph;

	std::shared_ptr<tgui::Button> addSegment;
	std::shared_ptr<tgui::Button> deleteSegment;
	std::shared_ptr<tgui::Label> startTimeLabel;
	std::shared_ptr<SliderWithEditBox> startTime;
	std::shared_ptr<tgui::Button> changeSegmentType;
	std::shared_ptr<tgui::ListBox> segmentTypePopup;

	std::shared_ptr<ListBoxScrollablePanel> segmentList; // Each item ID is the index of the segment in tfv's segment vector
	std::shared_ptr<tgui::Label> tfvFloat1Label;
	std::shared_ptr<NumericalEditBoxWithLimits> tfvFloat1Slider;
	std::shared_ptr<tgui::Label> tfvFloat2Label;
	std::shared_ptr<NumericalEditBoxWithLimits> tfvFloat2Slider;
	std::shared_ptr<tgui::Label> tfvFloat3Label;
	std::shared_ptr<NumericalEditBoxWithLimits> tfvFloat3Slider;
	std::shared_ptr<tgui::Label> tfvFloat4Label;
	std::shared_ptr<NumericalEditBoxWithLimits> tfvFloat4Slider;
	std::shared_ptr<tgui::Label> tfvInt1Label;
	std::shared_ptr<NumericalEditBoxWithLimits> tfvInt1Slider;

	std::shared_ptr<TFV> oldTFV; // Should never be modified after setTFV() is called
	std::shared_ptr<PiecewiseTFV> tfv;
	std::shared_ptr<TFV> selectedSegment;
	int selectedSegmentIndex = -1;
	float tfvLifespan; // Shouldn't be modified except by setTFV()

	/*
	Signal emitted when a change is made to the TFV being edited.
	Optional parameter: a pair of a shared_ptr to the old TFV object 
		and shared_ptr to the new TFV object
	*/
	tgui::SignalTFVPair onValueChange = { "ValueChanged" };

	// bool used to ignore signals to prevent infinite loops
	bool ignoreSignals = false;
	bool ignoreResizeSignal = false;

	void deselectSegment();
	void selectSegment(int index);
	void populateSegmentList();
};

/*
Used to edit EMPAAngleOffsets.

Signals:
	ValueChanged - emitted when a change is made to the EMPAAngleOffset being edited.
		Optional parameter: a pair of a shared_ptr to the old EMPAAngleOffset object
			and shared_ptr to the new EMPAAngleOffset object
*/
class EMPAAngleOffsetGroup : public HideableGroup {
public:
	EMPAAngleOffsetGroup(EditorWindow& parentWindow);
	static std::shared_ptr<EMPAAngleOffsetGroup> create(EditorWindow& parentWindow) {
		return std::make_shared<EMPAAngleOffsetGroup>(parentWindow);
	}

	/*
	Initialize this widget to offset's values.
	offset won't be modified by this widget.

	The old offset emitted in the ValueChanged signal will offset until the next setEMPAAngleOffset() call.
	*/
	void setEMPAAngleOffset(std::shared_ptr<EMPAAngleOffset> offset);

	tgui::Signal& getSignal(std::string signalName) override;

private:
	EditorWindow& parentWindow;

	std::shared_ptr<tgui::Label> offsetName;
	std::shared_ptr<tgui::Button> changeType;
	std::shared_ptr<tgui::ListBox> typePopup;
	std::shared_ptr<tgui::Label> xLabel;
	std::shared_ptr<tgui::Label> yLabel;
	std::shared_ptr<NumericalEditBoxWithLimits> x;
	std::shared_ptr<NumericalEditBoxWithLimits> y;

	std::shared_ptr<EMPAAngleOffset> oldOffset; // Should never be modified after setTFV() is called
	std::shared_ptr<EMPAAngleOffset> offset;

	/*
	Signal emitted when a change is made to the EMPAAngleOffset being edited.
	Optional parameter: a pair of a shared_ptr to the old EMPAAngleOffset object
		and shared_ptr to the new EMPAAngleOffset object
	*/
	tgui::SignalEMPAAngleOffsetPair onValueChange = { "ValueChanged" };

	// bool used to ignore signals to prevent infinite loops
	bool ignoreSignals = false;
	bool ignoreResizeSignal = false;

	void updateWidgets();
};

/*
A panel that can load Levels and play them while rendering it either normally or in debug mode.
A LevelPack must be loaded before anything can be done.
*/
class SimpleEngineRenderer : public tgui::Panel, public EventCapturable {
public:
	SimpleEngineRenderer(sf::RenderWindow& parentWindow, bool userControlleView = true, bool useDebugRenderSystem = true);
	inline static std::shared_ptr<SimpleEngineRenderer> create(sf::RenderWindow& parentWindow) {
		return std::make_shared<SimpleEngineRenderer>(parentWindow);
	}

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	bool handleEvent(sf::Event event) override;

	void loadLevelPack(std::string name);
	void loadLevelPack(std::shared_ptr<LevelPack> levelPack);

	void loadLevel(int levelIndex);
	void loadLevel(std::shared_ptr<Level> level);

	void pause();
	void unpause();

protected:
	std::shared_ptr<LevelPack> levelPack;
	mutable entt::DefaultRegistry registry;

	std::unique_ptr<MovementSystem> movementSystem;
	std::unique_ptr<RenderSystem> renderSystem;
	std::unique_ptr<CollisionSystem> collisionSystem;
	std::unique_ptr<DespawnSystem> despawnSystem;
	std::unique_ptr<EnemySystem> enemySystem;
	std::unique_ptr<SpriteAnimationSystem> spriteAnimationSystem;
	std::unique_ptr<ShadowTrailSystem> shadowTrailSystem;
	std::unique_ptr<PlayerSystem> playerSystem;
	std::unique_ptr<CollectibleSystem> collectibleSystem;
	std::unique_ptr<AudioPlayer> audioPlayer;

private:
	sf::RenderWindow& parentWindow;

	bool paused;

	std::unique_ptr<SpriteLoader> spriteLoader;
	std::unique_ptr<EntityCreationQueue> queue;

	sf::FloatRect viewportFloatRect, viewFloatRect;

	bool useDebugRenderSystem;

	bool userControlledView;
	std::unique_ptr<ViewController> viewController;
	sf::View viewFromViewController;

	void physicsUpdate(float deltaTime) const;
	void updateWindowView();
};

/*
A widget with tabs that, when one is selected, shows a Panel associated with that tab.
Warning: undefined behavior when there are multiple tabs with the same name, or when 
a tab has an empty string for a name.

This widget will automatically call the selected tab's handleEvent() when this
widget's handleEvent() is called.

This widget is intended to be used for a small number of tabs. It might get
laggy when there's a lot.
*/
class TabsWithPanel : public tgui::Group, public EventCapturable {
public:
	enum MoreTabsListAlignment {
		Left, // The right side of moreTabsList will be x-aligned with the right side of moreTabsButton
		Right // The left side of moreTabsList will be x-aligned with the left side of moreTabsButton.
	};

	/*
	parentWindow - the EditorWindow that this widget will be or has been added to
	*/
	TabsWithPanel(EditorWindow& parentWindow);
	inline static std::shared_ptr<TabsWithPanel> create(EditorWindow& parentWindow) {
		return std::make_shared<TabsWithPanel>(parentWindow);
	}

	/*
	tabName - the unique name of the new tab
	associatedPanel - the Panel that will be shown when the tab is selected
	autoSelect - whether to select the new tab immediately
	closeable - whether to have a button that can close this tab
	*/
	void addTab(std::string tabName, std::shared_ptr<tgui::Panel> associatedPanel, bool autoSelect = true, bool closeable = false);
	/*
	tabName - the unique name of the new tab
	associatedPanel - the Panel that will be shown when the tab is selected
	index - the new index of the tab
	autoSelect - whether to select the new tab immediately
	closeable - whether to have a button that can close this tab
	*/
	void insertTab(std::string tabName, std::shared_ptr<tgui::Panel> associatedPanel, int index, bool autoSelect = true, bool closeable = false);
	/*
	Selects a tab.
	tabName - the unique name of the tab
	*/
	void selectTab(std::string tabName);
	/*
	Removes the tab.
	tabName - the unique name of the tab
	*/
	void removeTab(std::string tabName);
	/*
	Removes all tabs.
	*/
	void removeAllTabs();
	/*
	Mark/Unmark a specific tab as requiring pressing "Yes" to a confirmation prompt before
	the tab is actually closed. This function does nothing if hasCloseButtons is false.
	By default, no tab requires a confirmation prompt.

	tabName - the unique name of the tab
	message - the string to be displayed in the confirmation prompt.
		Set to an empty string to disable the confirmation prompt.
	*/
	void setTabCloseButtonConfirmationPrompt(std::string tabName, std::string message);
	/*
	Returns whether the tab exists.
	tabName - the unique name of the tab
	*/
	bool hasTab(std::string tabName);

	/*
	Caches all currently added tabs so that the current set of tabs can be reopened at a later time.
	If tabsSetIdentifier already refers to some set of tabs, the old set of tabs will be overriden.

	tabsSetIdentifier - the unique identifier that will later be used to reopen the cached tabs
	*/
	void cacheTabs(std::string tabsSetIdentifier);
	/*
	Returns whether tabsSetIdentifier refers to any set of cached tabs.

	See cacheTabs() for more info on tabsSetIdentifier.
	*/
	bool isCached(std::string tabsSetIdentifier);
	/*
	Removes all current tabs and reopens the set of tabs referred to by tabsSetIdentifier.
	If tabsSetIdentifier does not refer to any set of tabs, this function will do nothing and return false.
	Otherwise, if it returns true.

	See cacheTabs() for more info on tabsSetIdentifier.
	*/
	bool loadCachedTabsSet(std::string tabsSetIdentifier);
	/*
	Removes the set of tabs that tabsSetIdentifier refers to from the cache.
	If tabsSetIdentifier does not refer to any set of tabs, this function will do nothing.

	See cacheTabs() for more info on tabsSetIdentifier.
	*/
	void removeCachedTabsSet(std::string tabsSetIdentifier);
	/*
	Clears the tabs cache.
	*/
	void clearTabsCache();

	tgui::Signal& getSignal(std::string signalName) override;
	/*
	Returns the unique name of the currently selected tab.
	*/
	std::string getSelectedTab();
	/*
	Sets the alignment of the list that appears upon clicking the
	more tabs button.
	*/
	void setMoreTabsListAlignment(MoreTabsListAlignment moreTabsListAlignment);

	bool handleEvent(sf::Event event);

private:
	// The value part of the key:value pairing in the tabsCache cache
	struct CachedTabsValue {
		std::string currentlySelectedTab;
		// Should be ordered the same as tabsOrdering at the time of caching
		// A vector of pairs, a pair for every tab: the tab name and the associated panel
		std::vector<std::pair<std::string, std::shared_ptr<tgui::Panel>>> tabs;
		// The confirmation prompt for every close button
		// If hasCloseButtons is false, this will be an empty vector
		// Otherwise, this contains the string to be displayed for every close button
		// confirmation prompt, with an empty string meaning a prompt shouldn't be displayed
		std::vector<std::string> closeButtonConfirmationPrompts;
	};
	const float EXTRA_HEIGHT_FROM_SCROLLBAR = 16;
	// Spaces appended to every tab name to make room for the close button
	// If font size changes, the number of spaces should be updated and every tab
	// name also should be updated accordingly (even the ones that are cached).
	// This is a lot of work, so it's better to just never change tabs' text size
	std::string tabNameAppendedSpaces;

	// The tab name (without the tabNameAppendedSpaces) of the tab who is being closed
	// and has its confirmation prompt up
	std::string closeButtonConfirmationPromptTargetTabShortenedName;

	EditorWindow& parentWindow;

	// The ScrollablePanel that serves as a container for the tabs object
	std::shared_ptr<tgui::ScrollablePanel> tabsContainer;
	// The actual tabs object
	std::shared_ptr<tgui::Tabs> tabs;
	// Maps tab name to the Panel that will be showed when the tab is selected
	std::map<std::string, std::shared_ptr<tgui::Panel>> panelsMap;
	// Tracks the order of the tabs in moreTabsList
	std::vector<std::string> tabsOrdering;
	// A vector of pairs of close button and the confirmation prompt message to be shown, one pair for every tab. 
	// The index for closeButtons should match the index for tabsOrdering.
	// An empty confirmation prompt message means no confirmation prompt will be shown if the close button is pressed.
	std::vector<std::pair<std::shared_ptr<tgui::Button>, std::string>> closeButtons;

	// Panel (that belongs to a tab) that is currently active
	std::shared_ptr<tgui::Panel> currentPanel;

	// Button that shows all tabs that can't fit in this widget
	std::shared_ptr<tgui::Button> moreTabsButton;
	// The ListBoxScrollablePanel that is shown when moreTabsButton is clicked
	std::shared_ptr<ListBoxScrollablePanel> moreTabsList;
	// Alignment of the moreTabsList
	MoreTabsListAlignment moreTabsListAlignment = MoreTabsListAlignment::Left;

	// Maps a tab set identifier string to a vector
	// panelsMap, tabsOrdering, the name of the tab that was open (empty string if none)
 	Cache<std::string, CachedTabsValue> tabsCache;

	// Signal emitted when a tab is closed via its close button
	// Optional parameter: the name of the tab being closed
	tgui::SignalString onTabClose = { "TabClosed" };

	void onTabSelected(std::string tabName);
	// Should be called whenever there's an update to the tabs
	void onTabsChange();
	// Called when a confirmation prompt for a close button is answered
	void onCloseButtonConfirmationPromptAnswer(bool confirmed);
	/*
	Mark/Unmark a specific tab as requiring pressing "Yes" to a confirmation prompt before
	the tab is actually closed. This function does nothing if hasCloseButtons is false.
	By default, no tab requires a confirmation prompt.

	index - the tab's index in tabs
	message - the string to be displayed in the confirmation prompt.
		Set to an empty string to disable the confirmation prompt.
	*/
	void setTabCloseButtonConfirmationPrompt(int index, std::string message);
	/*
	Create a close button for some tab.

	index - the tab's index in tabs
	*/
	void createCloseButton(int index);
};

/*
A timeline whose elements are clickable buttons.

Signals:
"ElementClicked" - emitted when an element is clicked; 
	Optional parameter: the index of the element as given in setElements() (int)
*/
class ClickableTimeline : public tgui::Group {
public:
	ClickableTimeline();
	static std::shared_ptr<ClickableTimeline> create() {
		return std::make_shared<ClickableTimeline>();
	}

	/*
	Sets the elements in this timeline. The duration of each element is assumed to be such that it ends right as 
	the next element begins. The only exception is the last element, whose duration ends at maxTime.

	itemStartTimes - the start time and text of every element;
		this vector must be sorted in nondecreasing order by the start time and no element can overlap in time
	maxDuration - the maximum time of the timeline
	*/
	void setElements(std::vector<std::pair<float, std::string>> elementStartTimes, float maxTime);

	/*
	Sets the elements in this timeline. The duration of each element must be given.
	The timeline's max time is assumed to be the last element's start time added to its duration.

	elementStartTimesAndDuration - a vector of tuples representing the start time, duration, and text of each element, respectively;
		this vector must be sorted in nondecreasing order by the start time and no element can overlap in time
	*/
	void setElements(std::vector<std::tuple<float, float, std::string>> elementStartTimesAndDuration);

	/*
	Sets the elements in this timeline. The duration of each element must be given.

	elementStartTimesAndDuration - a vector of tuples representing the start time, duration, and text of each element, respectively;
		this vector must be sorted in nondecreasing order by the start time and no element can overlap in time
	maxTime - the maximum time that can be displayed in the timeline
	*/
	void setElements(std::vector<std::tuple<float, float, std::string>> elementStartTimesAndDuration, float maxTime);

	tgui::Signal& getSignal(std::string signalName) override;

private:
	tgui::SignalInt onElementPressed = { "ElementPressed" };

	// A vector of tuples containing start time, duration, and corresponding Button of each element, in that order
	// This vector is not guaranteed to be sorted in any way
	std::vector<std::tuple<float, float, std::shared_ptr<tgui::Button>>> elements;

	std::shared_ptr<tgui::ScrollablePanel> panel;
	std::shared_ptr<tgui::RangeSlider> cameraController;

	// How much to scale the x-coordinates and width of every button in the panel by
	float buttonScalar = 1.0f;
	// Another scalar that does the same thing; value comes from cameraController
	float buttonScalarScalar = 1.0f;
	// The maximum time value of the timeline
	float maxTimeValue = 1.0f;

	void updateButtonsPositionsAndSizes();
};

/*
A tgui::Label that shows text over time rather than all at once.
*/
class TimedLabel : public tgui::Label {
public:
	inline TimedLabel(float charDelay = 1 / 9.0f) : charDelay(charDelay) {}
	static std::shared_ptr<TimedLabel> create(float charDelay = 1 / 9.0f) {
		return std::make_shared<TimedLabel>(charDelay);
	}

	void update(sf::Time elapsedTime) override;
	void setText(const sf::String& text);

private:
	// The full text to be shown
	std::string text;
	// Number of characters in text
	int textNumChars = 0;
	// Current number of visible characters
	int numVisibleChars = 0;
	// Time in seconds since the last char was shown
	float timeSinceLastChar = 0;
	// Time in seconds before the next character is shown
	float charDelay;
};

/*
A panel used to place markers on a 2D area. The ordering of the markers is maintained.

Note for me: the markers are stored internally with a negative y position than what is inserted
to maintain a standard coordinate system. getMarkerPositions() will return the originally inputted
positions.
*/
class MarkerPlacer : public tgui::Panel, public EventCapturable {
public:
	MarkerPlacer(sf::RenderWindow& parentWindow, sf::Vector2u resolution = sf::Vector2u(MAP_WIDTH, MAP_HEIGHT), int undoStackSize = 50);
	static std::shared_ptr<MarkerPlacer> create(sf::RenderWindow& parentWindow, sf::Vector2u resolution = sf::Vector2u(MAP_WIDTH, MAP_HEIGHT), int undoStackSize = 50) {
		return std::make_shared<MarkerPlacer>(parentWindow, resolution, undoStackSize);
	}

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	bool handleEvent(sf::Event event) override;

	void lookAt(sf::Vector2f pos);

	void setMarkers(std::vector<std::pair<sf::Vector2f, sf::Color>> markers);
	void setMarker(int index, sf::Vector2f position, sf::Color color);
	sf::CircleShape getMarker(int index);
	void insertMarker(int index, sf::Vector2f position, sf::Color color);
	void removeMarker(int index);
	void removeMarkers();
	std::vector<sf::Vector2f> getMarkerPositions();

	void setCircleRadius(float circleRadius);
	void setOutlineThickness(float outlineThickness);
	/*
	Sets the color that future markers from the "Add" button will be.
	Default is red.
	*/
	void setAddButtonMarkerColor(sf::Color color);

protected:
	// The color of the selected marker
	sf::Color selectedMarkerColor = sf::Color::Green;
	// The color of future markers from the "Add" button
	sf::Color addButtonMarkerColor = sf::Color::Red;

	virtual void setSelectedMarkerXWidgetValue(float value);
	virtual void setSelectedMarkerYWidgetValue(float value);
	virtual void updateMarkersListView();

private:
	const static std::string MARKERS_LIST_VIEW_ITEM_FORMAT;
	const sf::Vector2u resolution;
	sf::RenderWindow& parentWindow;
	UndoStack undoStack;

	// Widgets
	std::shared_ptr<tgui::ScrollablePanel> leftPanel;
	std::shared_ptr<ListViewScrollablePanel> markersListView;
	std::shared_ptr<tgui::Button> addMarker;
	std::shared_ptr<tgui::Button> deleteMarker;
	std::shared_ptr<tgui::Label> selectedMarkerXLabel;
	std::shared_ptr<tgui::Label> selectedMarkerYLabel;
	std::shared_ptr<NumericalEditBoxWithLimits> selectedMarkerX;
	std::shared_ptr<NumericalEditBoxWithLimits> selectedMarkerY;

	float circleRadius = 10.0f;
	float outlineThickness = 3.0f;
	std::vector<sf::CircleShape> markers;

	sf::FloatRect viewportFloatRect, viewFloatRect;

	std::unique_ptr<ViewController> viewController;
	sf::View viewFromViewController;

	// Radius will be < 0 if no cursor
	sf::CircleShape currentCursor;

	// Index of the selected marker. -1 if nothing is selected.
	int selectedMarkerIndex = -1;
	bool draggingMarker = false;
	// World pos of the placeholder being dragged at the moment it started being dragged
	sf::Vector2f markerPosBeforeDragging;

	// Set to true while left mouse is held down right after selecting a placeholder.
	// Used for preventing deselection from occurring in the same mouse press/release sequence that triggers selection.
	bool justSelectedMarker = false;

	// Screen coordinates of the mouse in the last MouseMove event while
	// draggingMarker was true
	int previousMarkerDragCoordsX, previousMarkerDragCoordsY;
	// Screen coordinates of the first left  mouse pressed event since the last left mouse released event
	int initialMousePressX, initialMousePressY;

	// Whether the user is currently placing a new marker. Should be modified only through setter.
	bool placingNewMarker = false;

	bool ignoreSignals = false;

	void selectMarker(int index);
	void setPlacingNewMarker(bool placingNewMarker);
	void deselectMarker();
	void updateWindowView();
	void updateMarkersListViewItem(int index);
};