import cv2
import numpy as np
from tensorflow.lite.python.interpreter import Interpreter

###DEPENDENCIES###
#pip install opencv-python


def visualize_keypoints(image, keypoints, xmin, ymin):
    img = image
    # Draw a red circle at each keypoint
    for current_keypoints in keypoints:
        for (x, y) in current_keypoints:
            cv2.circle(img,(int((x)), int(y)), 4, (0, 0, 255), -1)

    # Display the image with keypoints
    cv2.imshow("Image with Keypoints", img)

def keypoint_pipeline(frame, min_conf=0.5):


  # Load image and resize to expected shape [1xHxWx3]
  image_resized = cv2.resize(frame, (width, height),
                             interpolation=cv2.INTER_AREA)
  image_rgb = cv2.cvtColor(image_resized, cv2.COLOR_BGR2RGB)
  imH, imW, _ = frame.shape
  
  input_data = np.expand_dims(image_rgb, axis=0)

  input_data = (np.float32(input_data) - input_mean) / input_std

  # Perform the actual detection by running the model with the image as input
  interpreter.set_tensor(input_details[0]['index'],input_data)
  interpreter.invoke()

  # Retrieve detection results
  boxes = interpreter.get_tensor(output_details[1]['index'])[0] # Bounding box coordinates of detected objects
  scores = interpreter.get_tensor(output_details[0]['index'])[0] # Confidence of detected objects

  
  # Loop over all detections and draw detection box if confidence is above minimum threshold
  ymin = -1
  for i in range(len(scores)):
    if ((scores[i] > min_conf) and (scores[i] <= 1.0)):

     # Get bounding box coordinates and draw box
     # Interpreter can return coordinates that are outside of image dimensions, need to force them to be within image using max() and min()
     ymin = int(max(1,(boxes[i][0] * imH)))
     xmin = int(max(1,(boxes[i][1] * imW)))
     ymax = int(min(imH,(boxes[i][2] * imH)))
     xmax = int(min(imW,(boxes[i][3] * imW)))


  #Display the frame
  #Crop the image and detect keypoints
  if ymin != -1:
      crop_x_min = max(xmin - 125,1) 
      crop_x_max = min(xmax + 125,imW)
      crop_y_min = max(ymin - 125,1)
      crop_y_max = min(ymax + 125,imH)
          
      
      keypoint_input = frame[crop_y_min:crop_y_max,crop_x_min:crop_x_max]
      
      keypoint_input = cv2.resize(keypoint_input,(224,224),
                                  interpolation=cv2.INTER_AREA)
  
      #preprocess keypoint detection input
      keypoint_input = cv2.cvtColor(keypoint_input, cv2.COLOR_BGR2RGB)
      keypoint_input = keypoint_input.astype(np.float32)
      keypoint_input = np.expand_dims(keypoint_input, axis=0)
  

      keypoint_interpreter.set_tensor(keypoint_input_details[0]['index'],keypoint_input)
      keypoint_interpreter.invoke()
    
      keypoint_detections = keypoint_interpreter.get_tensor(
         keypoint_output_details[0]['index'])
      
      visualize_input = keypoint_detections.reshape(-1, 21, 2) * 224
      
      
      classifier_interpreter.set_tensor(classifier_input_details[0]['index'],keypoint_detections[0][0])
      classifier_interpreter.invoke()
      
      gesture = classifier_interpreter.get_tensor(
         classifier_output_details[0]['index'])
      
      gesture = np.argmax(gesture)
      
      labelmap = [
         "open_hand",
         "pointer_thumb",
         "middle_thumb",
         "ring_thumb",
         "pinky_thumb",
         "none"]
     
      
      visualize_keypoints(image_resized, visualize_input, 
                          crop_x_min, crop_y_min)
      if gesture != 5:
          f = open("..\\gestureSharing.txt", "w")
          f.write(labelmap[gesture])
          f.flush()
          f.close()
      print(labelmap[gesture])
  else:
      cv2.imshow("Image with Keypoints", image_resized)

  return

#Load the Tensorflow Lite models into memory
hand_model_path = "..\\Gesture-Recog-Pipeline\\detect.tflite"
keypoint_model_path = "..\\Gesture-Recog-Pipeline\\keypoint_detector.tflite"
gesture_classifier_model_path = "..\\Gesture-Recog-Pipeline\\Gesture_ClassifierV6.tflite"
#"..\\Gesture-Recog-Pipeline\\Gesture_ClassifierV4.tflite"

#"..\\Gesture-Recog-Pipeline\\Gesture_ClassifierV2.tflite"

interpreter = Interpreter(model_path=hand_model_path)
interpreter.allocate_tensors()
  
classifier_interpreter = Interpreter(model_path=gesture_classifier_model_path)
classifier_interpreter.allocate_tensors()

classifier_input_details = classifier_interpreter.get_input_details()
classifier_output_details = classifier_interpreter.get_output_details()

keypoint_interpreter = Interpreter(model_path=keypoint_model_path)
keypoint_interpreter.allocate_tensors()
  
keypoint_input_details = keypoint_interpreter.get_input_details()
keypoint_output_details = keypoint_interpreter.get_output_details()

# Get model details
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()
height = input_details[0]['shape'][1]
width = input_details[0]['shape'][2]
  
input_mean = 127.5
input_std = 127.5


    
#start capturing from webcam
vid = cv2.VideoCapture(0) 
    

while(True): 
      
# Capture the video frame 
    ret, frame = vid.read()   
    #frame = cv2.flip(frame,1)

    keypoint_pipeline(frame, min_conf=0.5)
        
    # 'q' to quit
    if cv2.waitKey(1) & 0xFF == ord('q'): 
        break
  
# After the loop release the cap object 
vid.release() 

# Destroy all the windows 
cv2.destroyAllWindows() 

