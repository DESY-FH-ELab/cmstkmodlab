#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include "ConradCommunication.h"
#include "ConradController.h"

//! Default constructor
ConradController::ConradController(const char* comPort)
  : VConradController(comPort),
    m_communication(new ConradCommunication(comPort))
{
}

//! Default destructor
ConradController::~ConradController()
{
    m_communication->finish();
    delete m_communication;
}

//! Initialize Conrad IO communication
bool ConradController::initialize()
{
    assert(m_communication);

    // Initialize communication
    if (!m_communication->initialize())
        return false;

    // Initialize card
    if (!m_communication->sendCommand(1, 1, 0))
        return false;

    // Check wheter initialization was successful
    unsigned char answer, address, status;
    if (!m_communication->receiveAnswer(&answer, &address, &status))
        return false;

    if (answer != 254) {
        fprintf(stderr, "[Conrad] ERROR: Card initialization failed! (Phase 1)\n");
        return false;
    }

    // Retrieve second answer from card
    if (!m_communication->receiveAnswer(&answer, &address, &status))
        return false;

    if (answer != 1) {
        fprintf(stderr, "[Conrad] ERROR: Card initialization failed! (Phase 2)\n");
        return false;
    }

    return true;
}

//! Internal helper function
static inline bool isBitSet(unsigned char data, unsigned bit)
{
    return (data & (1 << bit));
}

//! Internal helper function
bool ConradController::queryRawStatus(unsigned char& status) const
{
    // Request channel status
    if (!m_communication->sendCommand(2, 1, 0))
        return false;

    // Read result
    unsigned char answer, address;
    if (!m_communication->receiveAnswer(&answer, &address, &status))
        return false;

    if (answer != 253) {
        fprintf(stderr, "[Conrad] ERROR: Querying channel status failed!\n");
        return false;
    }

    return true;
}

//! Query channel status (returns 8 items reflecting the channel state, on/off, 0 items if it failed)
std::vector<bool> ConradController::queryStatus() const
{
    std::vector<bool> result;

    unsigned char status;
    if (!queryRawStatus(status))
        return result;

    result.resize(8);
    for (unsigned i = 0; i < 8; ++i)
        result[i] = isBitSet(status, i);

    return result;
}

//! Set a specific channel on or off, not touching any other channel status
bool ConradController::setChannel(unsigned channel, bool value) const
{
    assert(channel <= 8);
    unsigned twoPowChannel = (unsigned) pow(2.0, channel - 1.0);

    unsigned char channelMask;
    if (!queryRawStatus(channelMask))
        return false;

    if (value)
        channelMask |= twoPowChannel;
    else
        channelMask &= ~twoPowChannel;

    if (!m_communication->sendCommand(3, 1, channelMask))
        return false;

    // Check wheter switching was successful
    unsigned char answer, address, status;
    if (!m_communication->receiveAnswer(&answer, &address, &status))
        return false;

    if (answer != 252) {
        fprintf(stderr, "[Conrad] ERROR: Setting channel '%i' failed!\n", channel);
        return false;
    }

    return true;
}

//! Set a specific channel on or off, and switch _all_ other channels to off
bool ConradController::setSingleChannel(unsigned channel, bool value) const
{
    assert(channel <= 8);
    unsigned twoPowChannel = (unsigned) pow(2.0, channel - 1.0);

    unsigned char channelMask = 0;
    if (value)
        channelMask |= twoPowChannel;
    else
        channelMask &= twoPowChannel;

    if (!m_communication->sendCommand(3, 1, channelMask))
        return false;

    // Check wheter switching was successful
    unsigned char answer, address, status;
    if (!m_communication->receiveAnswer(&answer, &address, &status))
        return false;

    if (answer != 252) {
        fprintf(stderr, "[Conrad] ERROR: Setting channel '%i' failed!\n", channel);
        return false;
    }

    return true;
}
