#include "BeatDetector.hpp" 
#include "BeatEvent.hpp"

void BeatDetector::load() {
	engine = FMODAudioEngine::sharedEngine();
	system = engine->m_system;
	
	if (isPlaying()) {
		log::debug("[BeatDetector]: we playin' rn");
		const char* j = engine->m_musicChannels[1].m_filePath.c_str(); // idgaf imo
		sauce = new_aubio_source(j, samplerate, hopsize);
		if (samplerate == 0) aubio_source_get_samplerate(sauce);
	}
}

void BeatDetector::update() {
	fvec_t* in = new_fvec(hopsize);
	fvec_t* out = new_fvec(2);

	uint_t read;
	aubio_source_do(sauce, in, &read);
	if (read < hopsize) {
		// this is intended, dont modify it will break the code
		// edit: nvm
		// edit: actually i think reading the entire thing wouldn't cost anything isn't it
		log::debug("[Sayori] Master-sama! aubio does not read all the input I gave to him!");
	};
	aubio_onset_do(detector, in, out);

	// we got the beat, aubio docs said so idk if it even true
	if (out->data[0] != 0) {
		// post
		BeatEvent(dummyNode, aubio_onset_get_last_s(detector)).post();
	}
}