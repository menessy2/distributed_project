#include "Image.h"

Image::Image() {
}

Image::Image(const std::string& filename){
    filepath = filename;
}

Image::Image(const Image& orig) {
    set_string(orig.data_array, orig.size);
    filepath = orig.filepath;
}


