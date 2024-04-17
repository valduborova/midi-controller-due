## WIN

### Step 1: Locate Arduino Core Files for the Due

1. **Navigate to the Arduino IDE installation directory.**
   - Typically, this would be in `C:\Program Files (x86)\Arduino` on Windows, but it might vary depending on where you installed the Arduino IDE.

2. **Find the SAM core files.**
   - For the Arduino Due, which uses an Atmel SAM3X8E ARM Cortex-M3 CPU, the relevant core files are under the `hardware` directory. Depending on your Arduino IDE version and how you've installed the SAM boards, this could be in a path like `C:\Users\[Your User Name]\AppData\Local\Arduino15\packages\arduino\hardware\sam\1.6.x\` where `[Your User Name]` is your Windows username and `1.6.x` is the version of the SAM board package you have installed.

### Step 2: Modify the USB Descriptor

1. **Locate the USB descriptor files.**
   - Within the SAM core files, look for the USB descriptor file. The file path might differ slightly based on the version, but you should find it under something similar to `cores\arduino\USB`.

2. **Edit the USB descriptor file.**
   - You are looking for a file that contains the USB configuration, possibly named `USBCore.cpp` or similar. Open this file with a text editor like Notepad++ or Visual Studio Code.
   - Search for a line that defines the product string, such as `#define PRODUCT_STRING "Arduino Due"`.
   - Change `"Arduino Due"` to the desired name, ensuring that you keep the quotation marks. For example, `#define PRODUCT_STRING "My Custom MIDI Device"`.

3. **Save the file.**
   - After making your changes, save the file. Make sure you have the necessary permissions to edit and save the file; you might need administrative privileges.

### Step 3: Re-upload Your Sketch

1. **Compile and upload your sketch to the Arduino Due.**
   - Open your Arduino IDE, load your sketch, and upload it to your Arduino Due as usual.

2. **Check the device name.**
   - Once the upload is complete, disconnect and reconnect your Arduino Due to your computer. Check the device name to confirm it has been updated to your custom name in the MIDI software or device manager.

### Important Considerations

- **Backup original files**: Before making any modifications, it's a good practice to make backup copies of the original files you intend to edit.
- **Updates might overwrite changes**: Be aware that updating the Arduino IDE or the SAM board packages might overwrite your changes. You may need to reapply these changes after an update.

This process alters the USB descriptor information, which can be used to change how the device identifies itself for various USB functionalities, including MIDI. Always proceed with caution and ensure you are comfortable editing system files.

## MAC

On a Mac, the process to change the Arduino Due name with the MIDI USB library involves editing the board's core files similar to Windows, but the file paths will be different. Here's how you can go about it on macOS:

### Step 1: Locate the Arduino SAM Core Files

The SAM core files for Arduino Due are within the Arduino IDE installation directory or within the Arduino15 directory, depending on how you installed the Arduino IDE or the SAM boards.

1. **Arduino IDE installed via the Arduino website**:
    - The core files might be in your Applications folder. Right-click on the Arduino application, select "Show Package Contents," and navigate through `Contents` -> `Java` -> `hardware` -> `arduino` -> `sam`.

2. **Boards installed via Board Manager**:
    - You will likely find the SAM boards in the Arduino15 directory. Open Finder, press `Shift+Cmd+G`, and enter `~/Library/Arduino15/packages/arduino/hardware/sam/`. This will take you to the versions of the SAM board you have installed. If you have multiple versions, you’ll need to choose the one you are currently using.

### Step 2: Modify the USB Descriptor

1. **Find the USB descriptor files**:
    - Within the `sam` directory, navigate to `cores/arduino/USB`. Look for the file responsible for the USB descriptor, likely named something like `USBCore.cpp`.

2. **Edit the USB Descriptor**:
    - Use a text editor (such as TextEdit, Visual Studio Code, or Sublime Text) to open `USBCore.cpp`. You might need administrative privileges to edit this file.
    - Look for the product string definition. This might be defined with a line like `#define PRODUCT_STRING "Arduino Due"`.
    - Change the name from `"Arduino Due"` to your desired name, for example, `#define PRODUCT_STRING "My Custom MIDI Device"`, ensuring that the new name is within the quotation marks.
    - Save your changes.

### Step 3: Re-upload Your Sketch and Check

1. **Re-upload your sketch**:
    - Open the Arduino IDE, load your MIDI sketch, and upload it to your Arduino Due.

2. **Check the device name**:
    - After uploading, disconnect and then reconnect your Arduino Due to your Mac. You may check the new name in your MIDI software or the System Information under USB.

### Important Considerations

- **Backup**: Always backup the original files before making changes. You can simply copy and paste the original file in the same directory and rename it (e.g., `USBCore_backup.cpp`).
  
- **Updates may revert changes**: If you update the Arduino IDE or the SAM board packages via the Board Manager, your changes might be overwritten. You may need to redo the modifications after updates.

Doing this will change how your Arduino Due identifies itself to your Mac, making it show up with the new name you've given it. This process specifically alters the USB descriptors that the Arduino Due reports to the computer when it connects via USB.

Remember, modifying core files can potentially lead to issues if not done carefully. Always ensure that you have saved a backup of any file before you modify it, so you can restore it if needed.