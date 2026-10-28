#include "../include/Tagger.h"
#include "../include/Parser.h"
#include <taglib/tbytevector.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/tstringlist.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/urllinkframe.h>
#include <fstream>
#include <filesystem>
#include <format>
#include <array>

using namespace std::string_view_literals;
namespace po = boost::program_options;
namespace fs = std::filesystem;

constexpr std::array<const wchar_t*, 2> supported_types = {L".jpg", L".jpeg"};

bool Tagger::addPicture(const fs::directory_entry& mp3_path_entry, const std::wstring& song_name, const std::wstring& author, const std::wstring& image_folder) {
    try {
        Parser parser;
        TagLib::MPEG::File mp3_file(mp3_path_entry.path().c_str());
        TagLib::ID3v2::Tag* mp3_tag = mp3_file.ID3v2Tag(true);
        mp3_tag->removeFrames("APIC");

        std::wstringstream temp_stream;

        for (const auto& type : supported_types) {
            temp_stream.clear();
            temp_stream << image_folder << L'\\' << song_name << type;
            if (parser.fileExists(temp_stream.str())) {
                break;
            }
            temp_stream.clear();
            temp_stream << image_folder << L'\\' << author << type;
            if (parser.fileExists(temp_stream.str())) {
                break;
            }
        }

        if (!parser.fileExists(temp_stream.str())) {
            std::wcerr << std::format(L"No file named {}.jpg, {}.jpeg, {}.jpg or {}.jpeg in {}", song_name, song_name, author, author, image_folder) << L"\n";
            return false;
        }

        std::wstringstream image_stream;
        image_stream << temp_stream.rdbuf();
        std::ifstream image(std::filesystem::path(temp_stream.str()), std::ios::binary | std::ios::ate);
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
    } catch (...) {
        return false;
    }
}

bool Tagger::removeTags(const fs::directory_entry& mp3_path_entry) {
    try {
        TagLib::MPEG::File mp3_file(mp3_path_entry.path().c_str());
        TagLib::ID3v2::Tag* mp3_tag = mp3_file.ID3v2Tag(true);
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
    } catch (...) {
        return false;
    }
}

bool Tagger::tagFile(const fs::directory_entry& mp3_path_entry, const std::wstring& song_name, const std::wstring& author) {
    try {
        TagLib::MPEG::File mp3_file(mp3_path_entry.path().c_str());
        TagLib::ID3v2::Tag* mp3_tag = mp3_file.ID3v2Tag(true);
        auto* url = new TagLib::ID3v2::UserUrlLinkFrame();
        url->setUrl("https://github.com/framtk/mp3_tagger");
        url->setText("Git repo");
        mp3_tag->setTitle(song_name);
        mp3_tag->setArtist(author);
        mp3_tag->setComment("Tagged with mp3_tagger");
        mp3_tag->setAlbum(author + L" - " + song_name);
        mp3_tag->addFrame(url);
        mp3_file.save(TagLib::MPEG::File::ID3v2);
    } catch (...) {
        return false;
    }
    return true;
}

bool Tagger::apply(po::variables_map vm, const fs::directory_entry& file_path_entry) {
    try {
        bool verbose = vm.count("verbose") != 0;
        Parser parser;
        std::wstring song_name = file_path_entry.path().filename().wstring();
        song_name.erase(song_name.length() - 4);
        std::wstring author = parser.splitString(song_name, L'-')[0];
        std::wstring song_title = parser.splitString(song_name, L'-')[1];
        parser.trim(song_name);
        parser.trim(author);
        parser.trim(song_title);
        if (vm.count("clear")) {
            if (verbose) std::wcout << L"Clearing " << song_name << L" tags...\n";
            if (!removeTags(file_path_entry)) {
                std::wcerr << L"There was an error removing the tags of " << song_name << L"\n";
                return false;
            }
        }
        if (vm.count("tag")) {
            if (verbose) std::wcout << L"Tagging " << song_name << L"...\n";
            if (!tagFile(file_path_entry, song_title, author)) {
                std::wcerr << L"There was an error tagging " << song_name << L"\n";
                return false;
            }
        }
        if (vm.count("picture")) {
            std::wstring image_dir = parser.string_to_wstring(vm["picture"].as<std::string>());
            if (verbose) std::wcout << L"Setting " << song_name << L" picture...\n";
            if (!addPicture(file_path_entry, song_name, author, image_dir)) {
                std::wcerr << L"There was an error setting picture for " << song_name << L"\n";
                return false;
            }
        }
    } catch (...) {
        return false;
    }
    return true;
}
