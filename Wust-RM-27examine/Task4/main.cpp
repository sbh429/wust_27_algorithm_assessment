#include <iostream>
#include <cstdint>
#include <type_traits>
using namespace std;
namespace virt {
    class ImageSource {
        public:
            virtual ~ImageSource() = default;
            virtual int width() const = 0;
            virtual int height() const = 0;
            virtual unsigned char at(int x, int y) const = 0;
    };

    class CameraSource : public ImageSource {
        private:
            CameraFrame frame_;  
        public: explicit CameraSource(CameraFrame f):frame_(f){}
        int width() const override { return frame_.w; }
        int height() const override { return frame_.h; }
        unsigned char at(int x, int y) const override {
            return frame_.buf[y * frame_.w + x];
        }
        
    };
    class RosSource    : public ImageSource {
        private:
            RosImage frame_;
        public: explicit RosSource(RosImage f):frame_(f){}
        int width() const override { return frame_.w; }
        int height() const override { return frame_.h; }
        unsigned char at(int x, int y) const override{
            return frame_.data[y*frame_.step +x];
        }

    };
    class VideoSource  : public ImageSource {
        private:
            VideoFrame frame_;
        public: explicit VideoSource(VideoFrame f):frame_(f){}
        int width() const override { return frame_.w; }
        int height() const override { return frame_.h; }
        unsigned char at(int x, int y) const override{
            return frame_.max(yuv[y*w +x]-16,0);
        }
    };
    std::uint64_t preprocess(const ImageSource& src){
        std::uint64_t sum = 0;
        for(int y=0;y<src.height();y++){
            for(int x=0;x<src.width();x++){
                sum += src.at(x,y);
            }
        }
        return sum;
    }
}
namespace tmpl {
template <typename Frame>
std::uint64_t preprocess(const Frame& f){
    std::uint64_t sum = 0;
    for(int y=0;y<f.h;y++){
        for(int x=0;x<f.w;x++){
            if constexpr (std::is_same_v<Frame, CameraFrame>) {
                sum += f.buf[y * f.w + x];
            } else if constexpr (std::is_same_v<Frame, RosImage>) {
                sum += f.data[y * f.step + x];
            } else if constexpr (std::is_same_v<Frame, VideoFrame>) {
                sum += std::max(f.yuv[y * f.w + x] - 16, 0);
                sum += val>16 ?(val-16):0;
            }
        }
    }   
}return sum;
}