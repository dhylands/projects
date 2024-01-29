#!/usr/bin/env python3

def main():
    """
    Generates bioloid control data metadata from a C++ header file.
    """

import argparse
import os.path
import sys

from cxxheaderparser.simple import parse_file
from cxxheaderparser.visitor import NullVisitor
from cxxheaderparser.parser import CxxParser
from cxxheaderparser.types import Array
from cxxheaderparser.types import NameSpecifier
from cxxheaderparser.types import Token

PGM_NAME = os.path.basename(sys.argv[0])

class Visitor(NullVisitor):

    def __init__(self, class_name, verbose):
        self.control_table_class_name = class_name
        self.verbose = verbose
        self.in_control_table = False

    def get_segments_name(self, segments):
        for segment in segments:
            if isinstance(segment, NameSpecifier):
                return segment.name
        return ''

    def get_token_value(self, tokens):
        for token in tokens:
            if isinstance(token, Token):
                return token.value

    def get_class_name(self, state):
        return self.get_segments_name(state.class_decl.typename.segments)

    def get_type_name(self, type):
        if isinstance(type, Array):
            array_of = type.array_of
            array_size = self.get_token_value(type.size.tokens)
            return '{}[{}]'.format(self.get_type_name(array_of), array_size)
        return self.get_segments_name(type.typename.segments)

    def get_value(self, value):
        return value.tokens

    def on_class_start(self, state):
        class_name = self.get_class_name(state)
        print('on_class_start:', class_name)
        if class_name == self.control_table_class_name:
            self.in_control_table = True

    def on_class_field(self, state, field):
        if not self.in_control_table:
            return
        if self.verbose:
            print('on_class_field:', field)
        else:
            print('on_class_field:')
        print('  name:', field.name)
        print('  type:', self.get_type_name(field.type))

    def on_class_end(self, state):
        class_name = self.get_class_name(state)
        if class_name == self.control_table_class_name:
            self.in_control_table = False
        print('on_class_end:', class_name)


def main():
    parser = argparse.ArgumentParser(
        prog=PGM_NAME,
        usage='%(prog)s [options] header.h',
        description='Generates metadata for the bioloid control table'
    )
    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='Enable verbose logging'
    )
    parser.add_argument(
        'header',
        action='store',
        help='Name of header file to parse'
    )
    parser.add_argument(
        'class_name',
        action='store',
        help='Name of class defining the control table struct'
    )
    args = parser.parse_args(sys.argv[1:])

    if args.verbose:
        print('Header:', args.header)

    visitor = Visitor(args.class_name, args.verbose)
    hdr_parser = CxxParser(args.header, None, visitor)
    hdr_parser.parse();


if __name__ == "__main__":
    main()
