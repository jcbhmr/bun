#!/usr/bin/env bash
set -ex

tag="bun-v1.1.20"
curl -LO "https://github.com/oven-sh/bun/releases/download/$tag/bun-darwin-aarch64.zip"
curl -LO "https://github.com/oven-sh/bun/releases/download/$tag/bun-darwin-x64-baseline.zip"
curl -LO "https://github.com/oven-sh/bun/releases/download/$tag/bun-linux-aarch64.zip"
curl -LO "https://github.com/oven-sh/bun/releases/download/$tag/bun-linux-x64-baseline.zip"
curl -LO "https://github.com/oven-sh/bun/releases/download/$tag/bun-windows-x64-baseline.zip"
