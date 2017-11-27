import cv2
import time

initime = time.time()
cap = cv2.VideoCapture(0)
print(cap.isOpened())

while True:
  # Capture frame-by-frame. The ret parameter is useful
  # when reading a file that can have an end. Now we read
  # from the webcam, so we don't have this problem
  ret, image = cap.read()
  gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
  cv2.imshow("frame", image)
  if cv2.waitKey(1) & 0xFF == ord('q'):
    break
