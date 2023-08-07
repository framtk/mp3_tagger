//
// Created by francesco simone arrigo on 29.09.17.
//

#include "../include/Tagger.h"
#include "../include/Parser.h"
#include <taglib/tbytevector.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/tstringlist.h>
#include <taglib/attachedpictureframe.h>
#include <fstream>
#include <taglib/urllinkframe.h>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

std::vector<std::string> _supported_types = { ".jpg", ".jpeg" };

bool Tagger::addPicture(fs::directory_iterator mp3_path_itr, std::wstring song_name, std::wstring author, std::wstring image_folder) {
	try {

		Parser parser;

		TagLib::MPEG::File mp3_file(mp3_path_itr->path().c_str());
		TagLib::ID3v2::Tag* mp3_tag;
		mp3_tag = mp3_file.ID3v2Tag(true);

		// remove previous pictures to not have the mp3 file grow infinitely
		mp3_tag->removeFrames("APIC");

		std::wstringstream temp_stream;
		std::ifstream image;

		for (std::string type : _supported_types) {
			temp_stream.clear();
			temp_stream << image_folder << DIR_SEPARATOR << song_name << type.c_str();

			if (parser.fileExists(temp_stream.str())) {
				break;
			}

			temp_stream.clear();
			temp_stream << image_folder << DIR_SEPARATOR << author << type.c_str();

			if (parser.fileExists(temp_stream.str())) {
				break;
			}
		}

		if (!parser.fileExists(temp_stream.str())) {
			std::cerr << "No file named " << song_name << ".jpg" << ", " << song_name << ".jpeg" << ", " << author << ".jpg" << " or " << author << ".jpeg" << " in " << image_folder << "\n";
			return false;
		}

		std::wstringstream image_stream;

#ifdef _WIN32
		image_stream << temp_stream.rdbuf();
#else
		image_stream << parser.converter.to_bytes(temp_stream.str());
#endif

		image = std::ifstream(image_stream.str(), std::ios::binary | std::ios::ate);

		const auto fileSize = image.tellg();
		image.seekg(0);
		TagLib::ByteVector image_data((unsigned int)fileSize, 0);
		image.read(image_data.data(), fileSize);
		image.close();

		auto* picture = new TagLib::ID3v2::AttachedPictureFrame();
		picture->setMimeType("image/jpeg");
		picture->setDescription("Cover");
		picture->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
		picture->setPicture(image_data);
		mp3_tag->addFrame(picture);
		mp3_file.save(TagLib::MPEG::File::ID3v2);

		return true;

	} catch (std::exception) {
		return false;
	}
}

bool Tagger::removeTags(fs::directory_iterator mp3_path_itr) {

	try {

		TagLib::MPEG::File mp3_file(mp3_path_itr->path().c_str());
		TagLib::ID3v2::Tag* mp3_tag;
		mp3_tag = mp3_file.ID3v2Tag(true);

		mp3_tag->removeFrames("TCON");
		mp3_tag->removeFrames("TRCK");
		mp3_tag->removeFrames("TDRC");
		mp3_tag->removeFrames("TIT2");
		mp3_tag->removeFrames("TPE1");
		mp3_tag->removeFrames("COMM");
		mp3_tag->removeFrames("APIC");
		mp3_tag->removeFrames("WXXX");
		mp3_tag->removeFrames("TALB");

		mp3_file.save(TagLib::MPEG::File::ID3v2);

		return true;

	} catch (std::exception) {
		return false;
	}
}

bool Tagger::tagFile(fs::directory_iterator mp3_path_itr, std::wstring song_name, std::wstring author) {
	try {

		TagLib::MPEG::File mp3_file(mp3_path_itr->path().c_str());
		TagLib::ID3v2::Tag* mp3_tag;
		mp3_tag = mp3_file.ID3v2Tag(true);

		auto* url = new TagLib::ID3v2::UserUrlLinkFrame();
		url->setUrl("https://github.com/framtk/mp3_tagger");
		url->setText("Git repo");
		mp3_tag->setTitle(song_name);
		mp3_tag->setArtist(author);
		mp3_tag->setComment("Tagged with mp3_tagger");
		mp3_tag->setAlbum(author + " - " + song_name);
		mp3_tag->addFrame(url);

		mp3_file.save(TagLib::MPEG::File::ID3v2);

	} catch (std::exception) {
		return false;
	}
	return true;
}

bool Tagger::apply(po::variables_map vm, fs::directory_iterator file_path_itr) {

	try {
		bool verbose = vm.count("verbose") != 0;

		Parser parser;

		std::wstring song_name = file_path_itr->path().filename().wstring();
		song_name.erase(song_name.length() - 4);
		std::wstring author = parser.splitString(song_name, L'-')[0];
		std::wstring song_title = parser.splitString(song_name, L'-')[1];

		parser.trim(song_name);
		parser.trim(author);
		parser.trim(song_title);

		if (vm.count("clear")) {
			if (verbose)
				std::cout << "Clearing " << song_name << " tags...\n";
			if (!removeTags(file_path_itr)) {
				std::wcerr << "There was an error removing the tags of " << song_name << "\n";
				return false;
			};
		}


		if (vm.count("tag")) {
			if (verbose)
				std::cout << "Tagging " << song_name << "...\n";
			if (!tagFile(file_path_itr, song_title, author)) {
				std::wcerr << "There was an error tagging " << song_name << "\n";
				return false;
			}
		}

		if (vm.count("picture")) {
			std::wstring image_dir = parser.converter.from_bytes(vm["picture"].as<std::string>());
			if (verbose)
				std::wcout << "Setting " << song_name << " picture...\n";

			if (!addPicture(file_path_itr, song_name, author, image_dir)) {
				std::wcerr << "There was an error setting picture for " << song_name << "\n";
				return false;
			}
		}
	} catch (std::exception) {
		return false;
	}
	return true;
}
