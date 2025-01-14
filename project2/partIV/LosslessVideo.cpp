#include "LosslessVideo.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

LosslessVideo::LosslessVideo(const std::string& filePath, const std::string& codingType, int mValue)
    : videoPath(filePath), codingType(codingType), mValue(mValue), frameCount(0), optimalM(0), width(0), height(0) {
    format = extractFormat(filePath);
}

/*
void LosslessVideo::processVideo(const std::string& outputFile) {
    // Open video
    cv::VideoCapture video(videoPath);
    if (!video.isOpened()) {
        throw std::runtime_error("Failed to open video file: " + videoPath);
    }

    width = static_cast<int>(video.get(cv::CAP_PROP_FRAME_WIDTH));
    height = static_cast<int>(video.get(cv::CAP_PROP_FRAME_HEIGHT));
    frameCount = static_cast<int>(video.get(cv::CAP_PROP_FRAME_COUNT));

    BitStream bitStream(outputFile, true);

    // Placeholder for all residuals if mValue is zero
    std::vector<cv::Mat> allResiduals;

    cv::Mat frame;
    while (video.read(frame)) {
        cv::Mat residual;
        calculateResiduals(frame, residual);
        if (mValue == 0) {
            allResiduals.push_back(residual); // Store residuals for optimalM calculation
        } else {
            encodeResiduals(residual, bitStream); // Encode directly if mValue is provided
        }
    }

    if (mValue == 0) {
        optimalM = calculateOptimalM(allResiduals); // Calculate optimal m
        writeHeader(bitStream); // Write header with calculated optimal m

        // Encode all residuals
        for (const auto& residual : allResiduals) {
            encodeResiduals(residual, bitStream);
        }
    } else {
        writeHeader(bitStream); // Write header with provided m
    }

    bitStream.close();
    video.release();
    std::cout << "Video processing completed. Encoded file: " << outputFile << std::endl;
}*/
void LosslessVideo::processVideo(const std::string& outputFile) {
    // Open video
    cv::VideoCapture video(videoPath);
    if (!video.isOpened()) {
        throw std::runtime_error("Failed to open video file: " + videoPath);
    }

    width = static_cast<int>(video.get(cv::CAP_PROP_FRAME_WIDTH));
    height = static_cast<int>(video.get(cv::CAP_PROP_FRAME_HEIGHT));
    frameCount = static_cast<int>(video.get(cv::CAP_PROP_FRAME_COUNT));

    if (width <= 0 || height <= 0 || frameCount <= 0) {
        throw std::runtime_error("Invalid video properties. Check the video file.");
    }

    std::cout << "Video Details:\n"
              << "Number of frames: " << frameCount << "\n"
              << "Frame width: " << width << "\n"
              << "Frame height: " << height << "\n";

    // Check mValue and calculate optimalM if necessary
    if (mValue == 0) {
        mValue = calculateOptimalMFromVideo(video);
        if (mValue <= 0) {
            throw std::runtime_error("Calculated optimal m is invalid. m must be greater than 0.");
        }
    }

    std::cout << "Using Golomb parameter m: " << mValue << std::endl;

    BitStream bitStream(outputFile, true);
    writeHeader(bitStream);

    video.set(cv::CAP_PROP_POS_FRAMES, 0); // Reset video frame position
    cv::Mat frame;
    int currentFrame = 0;

    while (video.read(frame)) {
        currentFrame++;
        std::cout << "Processing frame " << currentFrame << " of " << frameCount << std::endl;

        if (frame.empty()) {
            std::cerr << "Error: Frame " << currentFrame << " is empty." << std::endl;
            throw std::runtime_error("Failed to read a frame from the video.");
        } else {
            std::cout << "Frame " << currentFrame << " read successfully with size: " 
                    << frame.rows << "x" << frame.cols << std::endl;
        }

        cv::Mat residual;
        calculateResiduals(frame, residual);

        if (residual.empty()) {
            throw std::runtime_error("Residuals matrix is empty for frame " + std::to_string(currentFrame));
        }

        try {
            encodeResiduals(residual, bitStream);
        } catch (const std::exception& ex) {
            throw std::runtime_error("Error encoding frame " + std::to_string(currentFrame) + ": " + ex.what());
        }
    }

    bitStream.close();
    video.release();
    std::cout << "Video processing completed. Encoded file: " << outputFile << std::endl;
}


/*
int LosslessVideo::calculateOptimalM(const std::vector<cv::Mat>& residuals) {
    double sum = 0;
    size_t count = 0;

    for (const auto& residual : residuals) {
        if (residual.channels() == 3) { // Color frame
            for (int y = 0; y < residual.rows; ++y) {
                for (int x = 0; x < residual.cols; ++x) {
                    for (int c = 0; c < 3; ++c) {
                        sum += std::abs(residual.at<cv::Vec3i>(y, x)[c]);
                    }
                }
            }
            count += residual.rows * residual.cols * 3;
        } else { // Grayscale frame
            for (int y = 0; y < residual.rows; ++y) {
                for (int x = 0; x < residual.cols; ++x) {
                    sum += std::abs(residual.at<int>(y, x));
                }
            }
            count += residual.rows * residual.cols;
        }
    }

    double mean = sum / count;
    int optimalM = 1;
    while (optimalM < mean) {
        optimalM *= 2;
    }

    return optimalM;
}
*/
int LosslessVideo::calculateOptimalMFromVideo(cv::VideoCapture& video) {
    double sum = 0;
    int count = 0;
    cv::Mat frame;

    video.set(cv::CAP_PROP_POS_FRAMES, 0); // Reset video to the first frame

    while (video.read(frame)) {
        if (frame.channels() == 3) { // Color frame
            for (int y = 0; y < frame.rows; ++y) {
                for (int x = 0; x < frame.cols; ++x) {
                    for (int c = 0; c < 3; ++c) {
                        sum += frame.at<cv::Vec3b>(y, x)[c]; // No std::abs needed for unsigned char
                    }
                }
            }
            count += frame.rows * frame.cols * 3;
        } else { // Grayscale frame
            for (int y = 0; y < frame.rows; ++y) {
                for (int x = 0; x < frame.cols; ++x) {
                    sum += frame.at<uchar>(y, x); // No std::abs needed for unsigned char
                }
            }
            count += frame.rows * frame.cols;
        }
    }

    if (count == 0) {
        throw std::runtime_error("Failed to calculate optimal m: No valid pixel data found.");
    }

    double mean = sum / count;
    int optimalM = 1;
    while (optimalM < mean) {
        optimalM *= 2;
    }

    return optimalM;
}


// Median Edge Predictor
int LosslessVideo::medianEdgePredictor(const cv::Mat& frame, int x, int y, int c) {
    int A, B, C;

    if (c == -1) { // Grayscale
        A = (x > 0) ? frame.at<uchar>(y, x - 1) : 0;     // Left neighbor
        B = (y > 0) ? frame.at<uchar>(y - 1, x) : 0;     // Top neighbor
        C = (x > 0 && y > 0) ? frame.at<uchar>(y - 1, x - 1) : 0; // Top-left neighbor
    } else { // Color channel
        const cv::Vec3b& pixelLeft = (x > 0) ? frame.at<cv::Vec3b>(y, x - 1) : cv::Vec3b(0, 0, 0);
        const cv::Vec3b& pixelTop = (y > 0) ? frame.at<cv::Vec3b>(y - 1, x) : cv::Vec3b(0, 0, 0);
        const cv::Vec3b& pixelTopLeft = (x > 0 && y > 0) ? frame.at<cv::Vec3b>(y - 1, x - 1) : cv::Vec3b(0, 0, 0);

        A = pixelLeft[c];
        B = pixelTop[c];
        C = pixelTopLeft[c];
    }

    if (C >= std::max(A, B))
        return std::min(A, B);
    else if (C <= std::min(A, B))
        return std::max(A, B);
    else
        return A + B - C;
}

// Calculate Residuals for a Frame
void LosslessVideo::calculateResiduals(const cv::Mat& frame, cv::Mat& residuals) {
    std::cout << "Calculating residuals for frame of size: " << frame.rows << "x" << frame.cols << std::endl;

    if (frame.channels() == 3) { // Color frame
        residuals = cv::Mat(frame.size(), CV_32SC3);
        for (int y = 0; y < frame.rows; ++y) {
            for (int x = 0; x < frame.cols; ++x) {
                for (int c = 0; c < 3; ++c) {
                    int actual = frame.at<cv::Vec3b>(y, x)[c];
                    int predicted = medianEdgePredictor(frame, x, y, c);
                    residuals.at<cv::Vec3i>(y, x)[c] = actual - predicted;
                }
            }
        }
    } else { // Grayscale frame
        residuals = cv::Mat(frame.size(), CV_32S);
        for (int y = 0; y < frame.rows; ++y) {
            for (int x = 0; x < frame.cols; ++x) {
                int actual = frame.at<uchar>(y, x);
                int predicted = medianEdgePredictor(frame, x, y, -1);
                residuals.at<int>(y, x) = actual - predicted;
            }
        }
    }

    if (residuals.empty()) {
        std::cerr << "Residuals matrix is empty after calculation." << std::endl;
        throw std::runtime_error("Residuals matrix calculation failed.");
    }
}

void LosslessVideo::writeHeader(BitStream& bitStream) {
    bitStream.writeBits(frameCount, 16); // Number of frames
    bitStream.writeBits(codingType == "POS_NEG" ? 1 : 0, 8); // Golomb coding type
    bitStream.writeBits(optimalM, 32); // Optimal m
    bitStream.writeBits(width, 32); // Video width
    bitStream.writeBits(height, 32); // Video height

    for (char ch : format) {
        bitStream.writeBits(static_cast<int>(ch), 8); // Write format
    }

    bitStream.writeBits('#', 8); // Header delimiter
}

void LosslessVideo::encodeResiduals(const cv::Mat& residuals, BitStream& bitStream) {
    std::cout << "Encoding residuals with size: " << residuals.rows << "x" << residuals.cols << std::endl;
    std::cout << "Using Golomb parameter m: " << mValue << std::endl;

    if (residuals.empty()) {
        throw std::runtime_error("Residuals matrix is empty in encodeResiduals.");
    }
    if (mValue <= 0) {
        throw std::runtime_error("Invalid Golomb parameter m. Must be > 0.");
    }

    GolombCoding encoder(mValue, bitStream, codingType == "POS_NEG" ? GolombCoding::POS_NEG : GolombCoding::SIGN_MAG);

    for (int y = 0; y < residuals.rows; ++y) {
        for (int x = 0; x < residuals.cols; ++x) {
            if (residuals.channels() == 3) {
                for (int c = 0; c < 3; ++c) {
                    int value = residuals.at<cv::Vec<int, 3>>(y, x)[c];
                    //std::cout << "Encoding value at (" << y << ", " << x << ", " << c << "): " << value << std::endl;
                    encoder.encode(value);
                }
            } else {
                int value = residuals.at<int>(y, x);
                //std::cout << "Encoding value at (" << y << ", " << x << "): " << value << std::endl;
                encoder.encode(value);
            }
        }
    }
}


std::string LosslessVideo::extractFormat(const std::string& filePath) {
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos != std::string::npos) {
        return filePath.substr(dotPos + 1);
    }
    return "unknown";
}

std::string LosslessVideo::getOriginalFormat(const std::string& encodedFile) {
    BitStream bitStream(encodedFile, false);

    // Skip header fields until format string
    bitStream.readBits(16); // Skip number of frames
    bitStream.readBits(8);  // Skip coding type
    bitStream.readBits(32); // Skip optimal m
    bitStream.readBits(32); // Skip video width
    bitStream.readBits(32); // Skip video height

    // Read format string
    std::string format;
    char ch;
    while ((ch = static_cast<char>(bitStream.readBits(8))) != '#') { // Stop at delimiter
        format += ch;
    }

    return format; // Return extracted format
}

void LosslessVideo::reconstructVideo(const std::string& inputFile, const std::string& outputFile) {
    BitStream bitStream(inputFile, false);
    readHeader(bitStream); // Ensure header is parsed correctly

    cv::VideoWriter writer(outputFile, cv::VideoWriter::fourcc('X', '2', '6', '4'), 30, cv::Size(width, height), true);
    if (!writer.isOpened()) {
        throw std::runtime_error("Failed to open video writer: " + outputFile);
    }

    std::cout << "Reconstructing video to: " << outputFile << std::endl;

    GolombCoding decoder(mValue, bitStream, codingType == "POS_NEG" ? GolombCoding::POS_NEG : GolombCoding::SIGN_MAG);

    for (int frameIndex = 0; frameIndex < frameCount; ++frameIndex) {
        std::cout << "Reconstructing frame " << frameIndex + 1 << " of " << frameCount << std::endl;

        cv::Mat residual(height, width, CV_32SC3); // Assuming color video
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (residual.channels() == 3) {
                    for (int c = 0; c < 3; ++c) {
                        residual.at<cv::Vec3i>(y, x)[c] = decoder.decode();
                    }
                } else {
                    residual.at<int>(y, x) = decoder.decode();
                }
            }
        }

        cv::Mat reconstructedFrame(height, width, CV_8UC3); // Assuming color video
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (residual.channels() == 3) {
                    for (int c = 0; c < 3; ++c) {
                        int predicted = medianEdgePredictor(reconstructedFrame, x, y, c);
                        int value = predicted + residual.at<cv::Vec3i>(y, x)[c];
                        reconstructedFrame.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(value);
                    }
                } else {
                    int predicted = medianEdgePredictor(reconstructedFrame, x, y, -1);
                    int value = predicted + residual.at<int>(y, x);
                    reconstructedFrame.at<uchar>(y, x) = cv::saturate_cast<uchar>(value);
                }
            }
        }

        writer.write(reconstructedFrame);
    }

    bitStream.close();
    writer.release();
    std::cout << "Reconstructed video saved to: " << outputFile << std::endl;
}


cv::Mat LosslessVideo::reconstructFrame(const cv::Mat& residual) {
    cv::Mat reconstructedFrame(residual.size(), CV_8UC3);

    for (int y = 0; y < residual.rows; ++y) {
        for (int x = 0; x < residual.cols; ++x) {
            for (int c = 0; c < 3; ++c) {
                int predicted = medianEdgePredictor(reconstructedFrame, x, y, c);
                int reconstructedValue = residual.at<cv::Vec3i>(y, x)[c] + predicted;

                // Clip the value to the valid range [0, 255]
                reconstructedFrame.at<cv::Vec3b>(y, x)[c] = std::clamp(reconstructedValue, 0, 255);
            }
        }
    }

    return reconstructedFrame;
}

void LosslessVideo::readHeader(BitStream& bitStream) {
    // Read the number of frames
    frameCount = bitStream.readBits(16);
    
    // Read the coding type
    int codingTypeFlag = bitStream.readBits(8);
    codingType = (codingTypeFlag == 1) ? "POS_NEG" : "SIGN_MAG";

    // Read the optimal m
    optimalM = bitStream.readBits(32);

    // Read video width and height
    width = bitStream.readBits(32);
    height = bitStream.readBits(32);

    // Read the format string
    format.clear();
    char ch;
    while ((ch = static_cast<char>(bitStream.readBits(8))) != '#') {
        format += ch;
    }

    std::cout << "Header Information:\n"
              << "Frame Count: " << frameCount << "\n"
              << "Coding Type: " << codingType << "\n"
              << "Optimal m: " << optimalM << "\n"
              << "Video Width: " << width << "\n"
              << "Video Height: " << height << "\n"
              << "Format: " << format << "\n";
}
