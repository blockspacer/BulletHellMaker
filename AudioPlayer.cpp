#include "AudioPlayer.h"
#include <algorithm>

//TODO: move these into some settings class
float masterVolume = 0.8f;
float soundVolume = 0.2f;
float musicVolume = 0.2f;

std::string SoundSettings::format() const {
	return formatString(fileName) + tos(volume) + tos(pitch) + formatBool(disabled);
}

void SoundSettings::load(std::string formattedString) {
	auto items = split(formattedString, DELIMITER);
	fileName = items[0];
	volume = std::stof(items[1]);
	pitch = std::stof(items[2]);
	disabled = unformatBool(items[3]);
}

std::string MusicSettings::format() const {
	return formatString(fileName) + formatBool(loops) + tos(loopStartMilliseconds) + tos(loopLengthMilliseconds) + tos(volume) + tos(pitch) + formatBool(disabled);
}

void MusicSettings::load(std::string formattedString) {
	auto items = split(formattedString, DELIMITER);
	fileName = items[0];
	loops = unformatBool(items[1]);
	loopStartMilliseconds = std::stoi(items[2]);
	loopLengthMilliseconds = std::stoi(items[3]);
	volume = std::stof(items[4]);
	pitch = std::stof(items[5]);
	disabled = unformatBool(items[6]);
}

void AudioPlayer::update(float deltaTime) {
	// Check if any sounds are done playing and remove it from the queue of sounds being played
	while (!currentSounds.empty() && currentSounds.front()->getStatus() == sf::Sound::Status::Stopped) {
		currentSounds.pop();
	}

	// Update music transitioning
	if (currentMusic && timeSinceMusicTransitionStart < musicTransitionTime) {
		currentMusic->setVolume((timeSinceMusicTransitionStart/musicTransitionTime) * musicTransitionFinalVolume * masterVolume * musicVolume);

		timeSinceMusicTransitionStart += deltaTime;
		if (timeSinceMusicTransitionStart >= musicTransitionTime) {
			// Prevent volume over/undershoot
			currentMusic->setVolume(musicTransitionFinalVolume * masterVolume * musicVolume);
		}
	}
}

/*
fileName - file name with extension
volume - in range [0, 100], where 100 is full volume
*/
void AudioPlayer::playSound(const SoundSettings& soundSettings) {
	if (soundSettings.isDisabled() || soundSettings.getFileName() == "") return;

	// Check if the sound's SoundBuffer already exists
	if (soundBuffers.count(soundSettings.getFileName()) == 0) {
		sf::SoundBuffer buffer;
		if (!buffer.loadFromFile(soundSettings.getFileName())) {
			//TODO: handle audio not being able to be loaded
			return;
		}
		soundBuffers[soundSettings.getFileName()] = std::move(buffer);
	}
	std::unique_ptr<sf::Sound> sound = std::make_unique<sf::Sound>();
	sound->setBuffer(soundBuffers[soundSettings.getFileName()]);
	sound->setVolume(soundSettings.getVolume() * masterVolume * soundVolume);
	sound->setPitch(soundSettings.getPitch());
	sound->play();
	currentSounds.push(std::move(sound));
}


/*
fileName - file name with extension
volume - in range [0, 100], where 100 is full volume
*/
std::shared_ptr<sf::Music> AudioPlayer::playMusic(const MusicSettings& musicSettings) {
	if (musicSettings.isDisabled() || musicSettings.getFileName() == "") return nullptr;

	std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
	if (!music->openFromFile(musicSettings.getFileName())) {
		//TODO: handle audio not being able to be loaded
		return nullptr;
	}
	music->setVolume(musicSettings.getVolume() * masterVolume * musicVolume);
	if (musicSettings.getLoop()) {
		music->setLoopPoints(sf::Music::TimeSpan(sf::milliseconds(musicSettings.getLoopStartMilliseconds()), sf::milliseconds(musicSettings.getLoopLengthMilliseconds())));
	}
	music->setPitch(musicSettings.getPitch());
	music->play();
	currentMusic = music;
	update(0);

	musicTransitionFinalVolume = musicSettings.getVolume();
	musicTransitionTime = musicSettings.getTransitionTime();
	timeSinceMusicTransitionStart = 0;

	return music;
}