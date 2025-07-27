#pragma once

/**
 * A non-atomic, static data structure that tells whether the appplication
 * should relaunch itself.
 * 
 * The reason behind this mechanic may be to autoreload its configurations. It
 * does not prompt the user or the system whether to reload, so once the value
 * is set to 1 or true, the system is expected to restart itself with no
 * hesitation.
 */
class ExitStatus {
public:
    // Get the current exit flag
    static bool get() {
        return status;
    }

    // Set the exit flag
    static void set(bool value) {
        status = value;
    }

private:
    static bool status;
};