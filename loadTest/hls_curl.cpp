/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * Source code using the multi interface to download many
 * files, with a capped maximum amount of simultaneous transfers.
 * </DESC>
 * Written by Michael Wallner
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#  include <unistd.h>
#endif
#include <curl/multi.h>

static const char *urls[] = {
      "http://vod.netflix.net/msd/elephant/elephant.ism/high.m3u8",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=354000.m3u8",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=354000-1.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=354000-2.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=551000.m3u8",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=551000-1.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=551000-2.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=551000-3.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000.m3u8",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-3.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-4.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-5.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-6.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-7.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-8.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-9.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-10.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-11.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-12.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-13.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-14.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-15.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-16.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-17.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-18.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-19.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-20.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-21.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-22.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-23.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-24.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-25.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-26.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-27.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-28.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-29.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-30.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-31.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-32.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-33.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-34.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-35.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-36.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-37.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-38.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-39.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-40.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-41.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-42.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-43.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-44.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-45.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-46.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-47.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-48.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-49.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-50.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-51.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-52.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-53.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-54.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-55.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-56.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-57.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-58.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-59.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-60.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-61.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-62.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-63.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-64.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-65.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-66.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-67.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-68.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-69.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-70.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-71.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-72.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-73.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-74.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-75.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-76.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-77.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-78.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-79.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-80.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-81.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-82.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-83.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-84.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-85.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-86.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-87.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-88.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-89.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-90.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-91.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-92.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-93.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-94.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-95.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-96.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-97.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-98.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-99.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-100.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-101.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-102.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-103.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-104.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-105.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-106.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-107.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-108.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-109.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-110.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-111.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-112.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-113.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-114.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-115.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-116.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-117.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-118.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-119.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-120.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-121.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-122.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-123.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-124.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-125.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-126.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-127.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-128.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-129.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-130.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-131.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-132.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-133.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-134.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-135.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-136.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-137.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-138.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-139.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-140.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-141.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-142.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-143.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-144.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-145.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-146.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-147.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-148.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-149.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-150.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-151.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-152.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-153.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-154.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-155.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-156.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-157.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-158.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-159.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-160.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-161.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-162.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-163.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-164.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-165.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-166.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-167.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-168.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-169.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-170.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-171.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-172.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-173.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-174.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-175.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-176.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-177.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-178.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-179.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-180.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-181.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-182.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-183.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-184.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-185.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-186.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-187.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-188.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-189.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-190.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-191.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-192.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-193.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-194.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-195.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-196.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-197.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-198.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-199.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-200.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-201.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-202.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-203.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-204.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-205.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-206.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-207.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-208.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-209.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-210.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-211.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-212.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-213.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-214.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-215.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-216.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-217.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-218.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-219.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-220.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-221.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-222.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-223.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-224.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-225.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-226.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-227.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-228.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-229.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-230.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-231.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-232.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-233.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-234.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-235.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-236.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-237.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-238.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-239.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-240.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-241.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-242.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-243.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-244.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-245.ts",
      "http://vod.netflix.net/msd/elephant/elephant.ism/elephant-audio_eng=64000-video_eng=807000-246.ts",
      //Earth Links -- 88 lines
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/sample.m3u8",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=591000.m3u8",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=591000-1.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=591000-2.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000.m3u8",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-1.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-2.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-3.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-4.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-5.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-6.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-7.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-8.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-9.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-10.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-11.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=3936000-12.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000.m3u8",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-2.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-3.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-4.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-5.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-6.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-7.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-8.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-9.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-10.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-11.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-12.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-13.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-14.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-15.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-16.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-17.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-18.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-19.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-20.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-21.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-22.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-23.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-24.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-25.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-26.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-27.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-28.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-29.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-30.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-31.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-32.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-33.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-34.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-35.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-36.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-37.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-38.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-39.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-40.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-41.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-42.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-43.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-44.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-45.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-46.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-47.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-48.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-49.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-50.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-51.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-52.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-53.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-54.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-55.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-56.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-57.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-58.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-59.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-60.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-61.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-62.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-63.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-64.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-65.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-66.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-67.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-68.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-69.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-70.ts",
      "http://vod.netflix.net/msd/netflix/multibitAudioSep/source_manifest.ism/source_manifest-audio_eng=128930-video_eng=7868000-71.ts",
};

#define MAX 10 /* number of simultaneous transfers */
#define CNT sizeof(urls)/sizeof(char *) /* total number of transfers to do */
size_t maxData =0;

static size_t cb(char *d, size_t n, size_t l, void *p)
{
  /* take care of the data here, ignored in this example */
  (void)d;
  (void)p;
  //printf("data size is(%Zu)\n", n);
  size_t temp = n*l;
  if(maxData < temp)
  {
    maxData = temp;
  }
  return temp;
}

static void init(CURLM *cm, int i)
{
  CURL *eh = curl_easy_init();

  curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, cb);
  curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
  curl_easy_setopt(eh, CURLOPT_URL, urls[i]);
  curl_easy_setopt(eh, CURLOPT_PRIVATE, urls[i]);
  curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);

  curl_multi_add_handle(cm, eh);
}

inline int usleep(useconds_t usecs)
{
    int ret;
    struct timespec req;
    struct timespec rem;
    req.tv_sec = usecs / 1000000;
    req.tv_nsec = (usecs % 1000000) * 1000;
    while(((ret = nanosleep(&req, &rem)) == -1) && (errno == EINTR)) {
      req = rem;
    }
    return ret;
}

int main(void)
{
  CURLM *cm;
  CURLMsg *msg;
  long L;
  unsigned int C=0;
  int M, Q, U = -1;
  fd_set R, W, E;
  struct timeval T;

  curl_global_init(CURL_GLOBAL_ALL);

  cm = curl_multi_init();

  /* we can optionally limit the total amount of connections this multi handle
     uses */
  curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)MAX);

  for (int x=0; x < 6; x++)
  {
    printf("next loop going to start\n");
    M= Q = U = -1;
    for(C = 0; C < MAX; ++C) {
      init(cm, C);
    }

    while(U) {
      curl_multi_perform(cm, &U);
      if(U) {
        FD_ZERO(&R);
        FD_ZERO(&W);
        FD_ZERO(&E);

        if(curl_multi_fdset(cm, &R, &W, &E, &M)) {
          fprintf(stderr, "E: curl_multi_fdset\n");
          return EXIT_FAILURE;
        }

        if(curl_multi_timeout(cm, &L)) {
          fprintf(stderr, "E: curl_multi_timeout\n");
          return EXIT_FAILURE;
        }
        if(L == -1)
          L = 100;
        if(M == -1) {
          printf("value of M is -1 sleeping for 200 ms\n");
          usleep(500000);
          //sleep((unsigned int)L / 1000);
        }
        else {
          T.tv_sec = L/1000;
          T.tv_usec = (L%1000)*1000;

          if(0 > select(M+1, &R, &W, &E, &T)) {
            fprintf(stderr, "E: select(%i,,,,%li): %i: %s\n",
                    M+1, L, errno, strerror(errno));
            return EXIT_FAILURE;
          }
        }
      }

      while((msg = curl_multi_info_read(cm, &Q))) {
        if(msg->msg == CURLMSG_DONE) {
          char *url;
          CURL *e = msg->easy_handle;
          curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
          if(msg->data.result != 0)
          {
            fprintf(stderr, "R: %d - %s <%s>\n",
                    msg->data.result, curl_easy_strerror(msg->data.result), url);
          }
          curl_multi_remove_handle(cm, e);
          curl_easy_cleanup(e);
        }
        else {
          fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
        }
        if(C < CNT) {
          init(cm, C++);
          U++; /* just to prevent it from remaining at 0 if there are more
                  URLs to get */
        }
      }

      //usleep(1000000);
      // printf("After reading response\n");
    }
  }
  printf("max data size(%Zu)\n", maxData);

  curl_multi_cleanup(cm);
  curl_global_cleanup();

  return EXIT_SUCCESS;
}
