#include <thread>

#include <opencv2/opencv.hpp>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;

enum View {
    ORIGINAL = 0,
    BLURRING = 1,
    GRAY = 2,
    CANNY = 3,
    COMICS = 4,

    END = 4
};

int main()
{
    cv::VideoCapture capture;
    capture.open(cv::VideoCaptureAPIs::CAP_ANY);
    if (!capture.isOpened()) {
        std::cerr << "error: cannot open video!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string name;
    cv::Mat source;
    cv::Mat image;
    bool run = true;
    View view = View::ORIGINAL;
    while (run) {
        const auto key = cv::waitKey(1);
        switch (key) {
            // End
            static constexpr int ESCAPE_KEY = 27;
        case ESCAPE_KEY:
            run = false;
            continue;

            // Change view
        case 'V':
        case 'v': {
            view = (View)((int)(view) + 1);
            if (view > View::END) {
                view = View::ORIGINAL;
            }
        } break;

        default:
            break;
        }

        if (capture.read(source) == false) {
            std::cerr << "error: read video!" << std::endl;
            return EXIT_FAILURE;
        }
        resize(source, source, cv::Size(WIDTH, HEIGHT), 0, 0);

        if (view == View::ORIGINAL) {
            image = std::move(source);
            name = "original";
        }
        if (view >= View::BLURRING) {
            const cv::Size ksize(5, 5); // Gaussian kernel size
            cv::GaussianBlur(source, image, ksize, 7, 7);
            name = "blurring";
        }
        if (view >= View::GRAY) {
            cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
            name = "gray";
        }
        if (view >= View::CANNY) {
            cv::Canny(image, image, 150, 300, 5, true);
            name = "canny";
        }
        if (view >= View::COMICS) {
            cv::Mat channels[3];
            cv::split(source, channels);
            channels[0] -= image;
            channels[1] -= image;
            channels[2] -= image;
            image = cv::Mat::zeros(image.rows, image.cols, CV_8UC3);
            cv::merge(channels, 3, image);
            name = "comics";
        }

        cv::putText(image, name, cv::Point(10, image.size().height - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.1, cv::Scalar(255, 0, 0), 1, 4);
        cv::imshow("widget", image);
    }

    capture.release();
    cv::destroyAllWindows();

    return EXIT_SUCCESS;
}
