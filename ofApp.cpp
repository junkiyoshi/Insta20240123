#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);

	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		if (glm::length(this->log_list[i].back()) > 1440) {

			this->log_list.erase(this->log_list.begin() + i);
			this->seed_list.erase(this->seed_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);

			continue;
		}

		auto feature = this->velocity_list[i] * 100;
		auto deg = ofMap(ofNoise(this->seed_list[i], ofGetFrameNum() * 0.005), 0, 1, 0, 360);
		feature += glm::vec2(40 * cos(deg * DEG_TO_RAD), 40 * sin(deg * DEG_TO_RAD));
		auto next = this->log_list[i].back() + glm::normalize(feature) * 15;

		this->log_list[i].push_back(next);

		while (this->log_list[i].size() > 30) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}

	ofColor color;
	auto radius = 50;
	for (int i = 0; i < 18; i++) {

		auto deg = ofRandom(360);
		auto next_deg = deg + 10;

		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		auto next = glm::vec2(radius * cos(next_deg * DEG_TO_RAD), radius * sin(next_deg * DEG_TO_RAD));

		vector<glm::vec2> log;
		log.push_back(location);
		this->log_list.push_back(log);

		this->seed_list.push_back(ofMap(ofNoise(glm::vec3(i, location * 0.001)), 0, 1, 0, 1000));
		this->velocity_list.push_back(glm::normalize(location - next) * 2);

		color.setHsb(ofRandom(255), ofRandom(200, 255), ofRandom(200, 255));
		this->color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	int sample_count = 180;
	for (int i = 0; i < this->log_list.size(); i++) {

		ofFill();
		ofSetColor(this->color_list[i]);

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	/*
	int start = 150;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}