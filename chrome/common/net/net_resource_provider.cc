// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/common/net/net_resource_provider.h"

#include <string>

#include "app/l10n_util.h"
#include "app/resource_bundle.h"
#include "base/string_piece.h"
#include "base/values.h"
#include "chrome/common/jstemplate_builder.h"
#include "grit/chromium_strings.h"
#include "grit/generated_resources.h"
#include "grit/net_resources.h"

namespace {

// The net module doesn't have access to this HTML or the strings that need to
// be localized.  The Chrome locale will never change while we're running, so
// it's safe to have a static string that we always return a pointer into.
// This allows us to have the ResourceProvider return a pointer into the actual
// resource (via a StringPiece), instead of always copying resources.
struct LazyDirectoryListerCacher {
  LazyDirectoryListerCacher() {
    DictionaryValue value;
    value.SetString(L"header",
                    l10n_util::GetString(IDS_DIRECTORY_LISTING_HEADER));
    value.SetString(L"parentDirText",
                    l10n_util::GetString(IDS_DIRECTORY_LISTING_PARENT));
    value.SetString(L"headerName",
                    l10n_util::GetString(IDS_DIRECTORY_LISTING_NAME));
    value.SetString(L"headerSize",
                    l10n_util::GetString(IDS_DIRECTORY_LISTING_SIZE));
    value.SetString(L"headerDateModified",
                    l10n_util::GetString(IDS_DIRECTORY_LISTING_DATE_MODIFIED));
    value.SetString(L"listingParsingErrorBoxText",
        l10n_util::GetStringF(IDS_DIRECTORY_LISTING_PARSING_ERROR_BOX_TEXT,
                              l10n_util::GetString(IDS_PRODUCT_NAME)));
    html_data = jstemplate_builder::GetI18nTemplateHtml(
        ResourceBundle::GetSharedInstance().GetRawDataResource(
            IDR_DIR_HEADER_HTML),
        &value);
  }

  std::string html_data;
};

}  // namespace

namespace chrome_common_net {

base::StringPiece NetResourceProvider(int key) {
  static LazyDirectoryListerCacher lazy_dir_lister;

  if (IDR_DIR_HEADER_HTML == key)
    return base::StringPiece(lazy_dir_lister.html_data);

  return ResourceBundle::GetSharedInstance().GetRawDataResource(key);
}

}  // namespace chrome_common_net
