# Safecheck

This repository is for an IOT school project.

Safecheck is a solution to the lack of connection in remote places during
trail running or hiking. It is a checkpoint system that allows the participants
to check in at each checkpoint and to be located in case of emergency.

The system is composed of a Raspberry Pi, a RFID reader, an emergency button,
a display and a Kinéis module (to send the data via satellite).

The supervisors can monitor the participants' progress on a web interface,
and know if any checkpoint has it's emergency button pressed.

# Modules

## Checkpoint

The checkpoint module is the one that will be installed at each checkpoint.
This code is to be executed on a Raspberry Pi model 3B or 3B+.
You can find the installation instructions in the [README](./checkpoint/README.md).

## Backend (Not implemented yet)

The supervisor's backend for the web interface.
It receives the data from the checkpoints from Kinéis and stores it in the database.
It also sends the data to the frontend.

## Frontend (Not implemented yet)

The supervisor's frontend for the web interface.
It displays the data from the backend.

## Database (Not implemented yet)

The database for the backend.
It stores the data from the checkpoints, including the participants' progress,
the emergency button presses, and the checkpoints' locations.
