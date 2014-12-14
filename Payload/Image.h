#ifndef IMAGE_H
#define	IMAGE_H

#include <string>

#include "Message.h"

class Image : public Message {
public:
    Image();
    Image(const std::string& filename);
    Image(const Image& orig);
    std::string get_filepath(){ return filepath; }
private:
    std::string filepath;

};

#endif	/* IMAGE_H */

