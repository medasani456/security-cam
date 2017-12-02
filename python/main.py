import cv2


def initialize_camera():
  global cap
  cap = cv2.VideoCapture(0)
  print(cap.isOpened())


def extract_background():
  exit_code, background = cap.read()
  cv2.imshow("background", background)
  cv2.waitKey(0)
  return background


def rgb2grey(image):
  return cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)


def run():

  # Extract background
  background = extract_background()
  grey_background = rgb2grey(background)

  while True:
    # Read new frame
    ret, image = cap.read()
    grey_image = rgb2grey(image)

    # Analize frame
    diff = cv2.absdiff(grey_image, grey_background)
    cv2.imshow("frame", diff)
    cv2.waitKey(0)

    # Binarize image with threshold
    max_pixel_value = 255
    threshold = 40
    th, diff2 = cv2.threshold(diff, threshold, max_pixel_value,
                              cv2.THRESH_BINARY)
    cv2.imshow("frame2", diff2)
    cv2.waitKey(0)

    # Low pass filter removes noise ?
    filtered = cv2.blur(diff2, (5, 5))
    cv2.imshow("frame3", filtered)
    cv2.waitKey(0)

    # Count different pixels to background and decide if detection
    zeros = cv2.countNonZero(filtered)
    if zeros > 100:
      print "DETECTION ??"


if __name__ == '__main__':
  initialize_camera()
  run()
