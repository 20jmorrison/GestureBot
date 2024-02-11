import cv2
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision

###DEPENDENCIES###
#pip install mediapipe
#pip install opencv-python
#need to download a model from https://developers.google.com/mediapipe/solutions/vision/gesture_recognizer/index


BaseOptions = mp.tasks.BaseOptions
GestureRecognizer = vision.GestureRecognizer
GestureRecognizerOptions = vision.GestureRecognizerOptions
GestureRecognizerResult = mp.tasks.vision.GestureRecognizerResult
VisionRunningMode = mp.tasks.vision.RunningMode



#handle what to do with the results
def print_result(result: GestureRecognizerResult, output_image: mp.Image, timestamp_ms: int):
    if result.gestures:
        print('gesture recognition result: {}'.format(result.gestures[0][0].category_name))

options = GestureRecognizerOptions(
    #load model downloaded from google mediapipe webpage
    base_options=BaseOptions(model_asset_path='C:\\Users\\gbell\\OneDrive\\Desktop\\SNHU\\Spring-2024\\AI\\custom_gesture_recognizer.task'),
    #set running mode to livestream
    running_mode=VisionRunningMode.LIVE_STREAM,
    result_callback=print_result)

with GestureRecognizer.create_from_options(options) as recognizer:
    
    #start capturing from webcam
    vid = cv2.VideoCapture(0) 
    
    #get fps to later calculate timestamp of each frame passed to gesture recognizer
    fps = vid.get(cv2.CAP_PROP_FPS)
    frame_count = 0

    while(True): 
      
        # Capture the video frame 
        ret, frame = vid.read() 
        
        #calculate time_stamp
        frame_count += 1
        time_stamp = (frame_count * 1000) / fps

        #preprocess frame by converting numpy array data returned from opencv to mediapipe image
        mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=frame)
        
        #detect gestures, automatically calls print_results and prints to the console
        recognizer.recognize_async(mp_image, int(time_stamp))    
        
        #Display the frame
        cv2.imshow('Gesture Recognition', frame) 
       
        # 'q' to quit
        if cv2.waitKey(1) & 0xFF == ord('q'): 
            break
  
    # After the loop release the cap object 
    vid.release() 
    # Destroy all the windows 
    cv2.destroyAllWindows() 
