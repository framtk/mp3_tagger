//
// Created by francesco simone arrigo on 29.09.17.
//

#include "../include/Tagger.h"
#include <boost/program_options.hpp>
#include <taglib/tbytevector.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>
#include <fstream>

bool Tagger::tagFile(std::string mp3_path, std::string song_name, std::string image_folder) {
    try {
        TagLib::MPEG::File mp3_file(mp3_path.c_str());
        TagLib::ID3v2::Tag *mp3_tag;

        mp3_tag = mp3_file.ID3v2Tag(true);

        std::stringstream stream;
        stream << image_folder << song_name << ".jpg";
        std::ifstream image(stream.str(), std::ios::binary | std::ios::ate);
        stream.str(std::string());

        const auto fileSize = image.tellg();
        image.seekg(0);
        TagLib::ByteVector image_data((unsigned int) fileSize, 0);
        image.read(image_data.data(), fileSize);
        image.close();

        auto *picture = new TagLib::ID3v2::AttachedPictureFrame();
        picture->setMimeType("image/jpeg");
        picture->setDescription("Cover");
        picture->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
        picture->setPicture(image_data);
        mp3_tag->addFrame(picture);
        mp3_file.save();
    } catch (std::exception &e){
        return false;
    }
    return false;
}
