# Copyright 2019,2020 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#

"""License text"""

from .cli import cloud, pass_context


@cloud.group()
@pass_context
def ibm(cli_ctx):
    """ (Not Implemented) IBM Watson Specific utilities

    This helps to handle ibm specific settings."""
    cli_ctx.vlog("IBM Watson Specific utilities")


@cloud.group()
@pass_context
def gcp(cli_ctx):
    """ (Not Implemented) GCP (Google Cloud Platform) Specific utilities

    This helps to handle GCP specific settings."""
    cli_ctx.vlog("GCP (Google Cloud Platform) Specific utilities")


@cloud.group()
@pass_context
def aws(cli_ctx):
    """ (Not Implemented) AWS (Amazon Web Services) Specific utilities

    This helps to handle AWS specific settings."""
    cli_ctx.vlog("AWS (Amazon Web Services) Specific utilities")
