#pragma once

void startffmpeg(GLFWwindow* window, renderContext& context);

void record(renderContext& context, int currentWidth, int currentHeight);

void closeRecorder(renderContext& context);